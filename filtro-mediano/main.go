package main

import (
	"fmt"
	"os"
	"runtime"
	"strconv"
	"strings"

	"filtro-mediano/createlog"
	"filtro-mediano/equalimg"
	"filtro-mediano/medianfilter"
	"filtro-mediano/medianfilterparalelo"
	"filtro-mediano/saltandpepper"
)

// Aplicar o salt and pepper na imagem de entrada
const APPLY_SALTANDPEPPER = true

// Automáticamente pegar o número de CPU de threads, se não, colocar manualmente via CLI
const NTHREADS_AUTOMATIC = false

// Executar o algoritmo sequencial
const EXEC_SEQ = true

// Executar o algoritmo paralelo
const EXEC_PARALELO = true

// Comparar as duas imagens de saída, sequencial e paralela
const COMPARE_IMG = false

// Fazer o log da duracao
const LOG_DURATION = true

func main(){
	nArgumentos := 4
	nThreads := 0

	if NTHREADS_AUTOMATIC {
		nArgumentos = 3
		nThreads = runtime.NumCPU()
	}

	if(len(os.Args) < nArgumentos) {
		fmt.Printf("Execute da seguinte maneira: %s <caminhoImagemEntrada> <tamanhoJanela> ", "go run main.go")
		if !NTHREADS_AUTOMATIC {
			fmt.Print("<nThreads>\n")
		} else {
			fmt.Print("\n")
		}
		os.Exit(1)
	}

	if !NTHREADS_AUTOMATIC {
		nThreads, _ = strconv.Atoi(os.Args[3])
	}

    // Pega o caminho da imagem do primeiro argumento passado
    caminhoImagemEntrada := os.Args[1]
	nomeImagem := strings.Split(caminhoImagemEntrada, ".")[0]
	extImagem := strings.Split(caminhoImagemEntrada, ".")[1]
	caminhoImagemSalt := nomeImagem + "_salt." + extImagem
	caminhoImagemSaida := nomeImagem + "_median." + extImagem
	caminhoImagemSaidaParalelo := nomeImagem + "_median_paralelo." + extImagem

    // Pega o tamanho da janela do segundo argumento passado
    tamanhoJanela, err := strconv.Atoi(os.Args[2])
	if err != nil {
		panic(err)
	}

	if tamanhoJanela % 2 == 0 {
		fmt.Println("Tamanho de janela não é ímpar.")
		os.Exit(1)
	}

	if APPLY_SALTANDPEPPER {
		saltandpepper.ApplySaltAndPepper(caminhoImagemEntrada, caminhoImagemSalt)
	} else {
		caminhoImagemSalt = caminhoImagemEntrada
	}

	logFileName := nomeImagem + "_janela" + strconv.Itoa(tamanhoJanela)

	if EXEC_SEQ {
		duracaoSeq := medianfilter.MedianFilter(caminhoImagemSalt, caminhoImagemSaida, tamanhoJanela)
		if LOG_DURATION {
			logFileSeq := logFileName + "_seq"
			createlog.CreateLog(duracaoSeq, logFileSeq)
		}
	}

	if EXEC_PARALELO {
		duracaoPar := medianfilterparalelo.MedianFilter(caminhoImagemSalt, caminhoImagemSaidaParalelo, tamanhoJanela, nThreads)
		if LOG_DURATION {
			logFilePar := logFileName + "_threads" + strconv.Itoa(nThreads)
			createlog.CreateLog(duracaoPar, logFilePar)
		}
	}

	if COMPARE_IMG {
		equalimg.CompararImagens(caminhoImagemSaida, caminhoImagemSaidaParalelo)
	}
}