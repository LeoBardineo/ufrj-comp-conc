package medianfilterparalelo

import (
	"fmt"
	"image"
	"image/color"
	"image/jpeg"
	_ "image/png"
	"log"
	"os"
	"sort"
	"strconv"
	"strings"
	"sync"
	"time"
)

const DEBUG = true
const REGIST_TIMER = true

func MedianFilter(caminhoImagemEntrada string, caminhoImagemSaida string, tamanhoJanela int, nThreads int) {
    inicioTotal := time.Now()
    // Abre a imagem de entrada de acordo com o camminho especificado
    arq, err := os.Open(caminhoImagemEntrada)
    if err != nil {
        panic(err)
    }

    // Aqui o uso de defer é utilizado para que o arquivo será fechado devidamente ao final do programa,
    // mesmo se ocorrer um erro ou panic, o uso do defer se repete ao longo desse e outros arquivos
    defer arq.Close()

    // Carrega a imagem, fazendo o decode de acordo com o tipo da imagem
	img, _, err := image.Decode(arq)
	if err != nil {
		log.Fatalf("Erro ao decodificar a imagem: %v", err)
	}

    // Aplica o filtro mediana na imagem
    filteredImg := aplicaMedianFilter(img, tamanhoJanela, nThreads)

	// Cria um arquivo para salvar a nova imagem
	outFile, err := os.Create(caminhoImagemSaida)
	if err != nil {
		log.Fatalf("Erro ao criar o arquivo de saída: %v", err)
	}
	defer outFile.Close()

	// Realiza o encode para o tipo da imagem e a salva
	err = jpeg.Encode(outFile, filteredImg, nil)
	if err != nil {
		log.Fatalf("Erro ao salvar a imagem: %v", err)
	}

    duracaoTotal := time.Since(inicioTotal)
    duracaoStr := strconv.FormatFloat(duracaoTotal.Seconds(), 'f', 2, 64)
    
    if REGIST_TIMER {
        logfile := strings.Split(caminhoImagemEntrada, ".")[0] + "_janela" + strconv.Itoa(tamanhoJanela) + "_threads" + strconv.Itoa(nThreads)
        arquivo, err := os.OpenFile("logs/"+logfile+".out", os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0777)
        if err != nil {
            log.Fatal("Erro ao abrir ou criar o arquivo:", err)
        }
        defer arquivo.Close()
        
        if _, err := arquivo.WriteString(duracaoStr + "\n"); err != nil {
            log.Fatal("Erro ao criar arquivo", err)
        }
    }

    if DEBUG {
        fmt.Printf("Filtro mediana aplicado com sucesso na imagem \"%s\" e salvo em \"%s\"!\n", caminhoImagemEntrada, caminhoImagemSaida)
    }
}

func aplicaMedianFilter(img image.Image, tamanhoJanela int, nThreads int) image.Image {
    // Pega a dimensão da imagem
    bounds := img.Bounds()

    // Prepara uma nova imagem RGB para salvar os novos valores dos pixels
    // com as mesmas dimensões da imagem original
    filteredImg := image.NewRGBA(bounds)

    var wg sync.WaitGroup
	linhasPorThread := (bounds.Max.Y - bounds.Min.Y) / nThreads

    // Para cada thread, executa uma goroutine que irá realiazr a mediana dos pixels
    // para suas respectivas linhas
	for i := 0; i < nThreads; i++ {
        inicioLinha := bounds.Min.Y + i*linhasPorThread
		fimLinha := inicioLinha + linhasPorThread

        // Se for a última thread, vai até o final da imagem,
        // cobrindo toda a imagem mesmo se a divisão da altura por threads tiver resto
		if i == nThreads - 1 {
            fimLinha = bounds.Max.Y
		}
        
		wg.Add(1)
		go func(startY, endY int) {
            defer wg.Done()
            // Percorre cada pixel, aplica a mediana dos seus vizinhos
            // e salva o valor na imagem criada anteriormente
			for y := startY; y < endY; y++ {
				for x := bounds.Min.X; x < bounds.Max.X; x++ {
					filteredImg.Set(x, y, medianPixel(img, x, y, tamanhoJanela))
				}
			}
		}(inicioLinha, fimLinha)
	}

	wg.Wait()

    return filteredImg
}

func medianPixel(img image.Image, x, y, tamanhoJanela int) color.Color {
    // Determina os limites, para não acessar um pixel fora das dimensões da imagem
    maxX, maxY := img.Bounds().Max.X, img.Bounds().Max.Y

    // Array de unsigned int de 8 bits, já que as cores RGBA vão de 0 a 255,
    // que será usado para armazenar os respectivos valores dos pixels vizinhos
    var r, g, b, a []uint8

    // Pega o tamanho da janela e decide as índices dos pixels das extremidades relativo ao pixel escolhido
    cantoDireito := (tamanhoJanela - 1)/2
    cantoEsquerdo := cantoDireito * -1

    // Percorre os pixels da janela utilizando índices relativo ao pixel escolhido
    for dy := cantoEsquerdo; dy <= cantoDireito; dy++ {
        for dx := cantoEsquerdo; dx <= cantoDireito; dx++ {
            // Pega o X e o Y do píxel vizinho escolhido para essa iteração
            nx, ny := x + dx, y + dy

            // Se nx e ny estiverem dentro dos limites da imagem,
            // salva os valores RGBA do píxel nessa posição nos arrays
            if nx >= 0 && nx < maxX && ny >= 0 && ny < maxY {
                pixel := img.At(nx, ny)

                // Como o RGBA retorna 16 bits, por motivos de compatibilidade com diferentes tipos de imagem,
                // é feito um shift de 8 bits para se adequar à função color.RGBA
                pr, pg, pb, pa := pixel.RGBA()
                r = append(r, uint8(pr>>8))
                g = append(g, uint8(pg>>8))
                b = append(b, uint8(pb>>8))
                a = append(a, uint8(pa>>8))
            }
        }
    }

    // Ordena os valores de RGBA dos pixels da janela
    sort.Slice(r, func(i, j int) bool { return r[i] < r[j] })
    sort.Slice(g, func(i, j int) bool { return g[i] < g[j] })
    sort.Slice(b, func(i, j int) bool { return b[i] < b[j] })
    sort.Slice(a, func(i, j int) bool { return a[i] < a[j] })

    // Pega o índice do meio da lista, conseguindo assim o respectivo valor mediano
    medianIndex := len(r) / 2

    return color.RGBA{
        R: r[medianIndex],
        G: g[medianIndex],
        B: b[medianIndex],
        A: a[medianIndex],
    }
}
