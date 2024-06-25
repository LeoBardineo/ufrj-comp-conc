package saltandpepper

import (
	"fmt"
	"image"
	"image/color"
	"image/jpeg"
	_ "image/png"
	"log"
	"math/rand"
	"os"
	"time"
)

const DEBUG = true

// Taxa de pixels que serão aplicado o salt and pepper
const taxaSaltPepper = 0.02

func ApplySaltAndPepper(caminhoImagemSaltEntrada string, caminhoImagemSaltSaida string) {
	// Abre a imagem de entrada de acordo com o camminho especificado
	arq, err := os.Open(caminhoImagemSaltEntrada)
	if err != nil {
		log.Fatalf("Erro ao abrir o arquivo: %v", err)
	}
	defer arq.Close()

	// Carrega a imagem, fazendo o decode de acordo com o tipo da imagem
	img, _, err := image.Decode(arq)
	if err != nil {
		log.Fatalf("Erro ao decodificar a imagem: %v", err)
	}

	// Obtem as dimensões da imagem
	bounds := img.Bounds()
	width, height := bounds.Dx(), bounds.Dy()

	// Cria uma nova imagem para o efeito salt and pepper
	saltPepperImg := image.NewRGBA(image.Rect(0, 0, width, height))

	// Configura o seed para o gerador de números aleatórios
	rand.Seed(time.Now().UnixNano())

	// Percorrer cada pixel da imagem
	for y := 0; y < height; y++ {
		for x := 0; x < width; x++ {
			// Obter a cor original do pixel
			corOriginal := img.At(x, y)

			// Gerar um número aleatório entre 0 e 1
			rnd := rand.Float64()

			// Aplica o efeito salt and pepper de acordo com o número aleatório
			// Se for menor que a metade da taxa, aplica branco (salt)
			// Se for maior que a metade da taxa, aplica preto (pepper)
			// Se não, mantem a cor original
			if rnd < taxaSaltPepper/2 {
				saltPepperImg.Set(x, y, color.White)
			} else if rnd > 1 - taxaSaltPepper/2 {
				saltPepperImg.Set(x, y, color.Black)
			} else {
				saltPepperImg.Set(x, y, corOriginal)
			}
		}
	}

	// Cria um arquivo para salvar a nova imagem
	outFile, err := os.Create(caminhoImagemSaltSaida)
	if err != nil {
		log.Fatalf("Erro ao criar o arquivo de saída: %v", err)
	}
	defer outFile.Close()

	// Codifica e salva a nova imagem
	err = jpeg.Encode(outFile, saltPepperImg, nil)
	if err != nil {
		log.Fatalf("Erro ao salvar a imagem: %v", err)
	}

	if DEBUG {
		fmt.Printf("Efeito salt and pepper aplicado com sucesso na imagem \"%s\" salvo em \"%s\"!\n", caminhoImagemSaltEntrada, caminhoImagemSaltSaida)
	}
}
