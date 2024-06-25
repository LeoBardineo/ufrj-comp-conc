package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"

	"filtro-mediano/medianfilter"
	"filtro-mediano/saltandpepper"
)

const APPLY_SALTANDPEPPER = true

func main(){
	if(len(os.Args) < 3) {
		fmt.Printf("Execute da seguinte maneira: %s <caminhoImagemEntrada> <tamanhoJanela>\n", "go run main.go")
		os.Exit(1)
	}

    // Pega o caminho da imagem do primeiro argumento passado
    caminhoImagemEntrada := os.Args[1]
	caminhoImagemSalt := strings.Split(caminhoImagemEntrada, ".")[0] + "_salt.jpg"
	caminhoImagemSaida := strings.Split(caminhoImagemEntrada, ".")[0] + "_median.jpg"

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

	medianfilter.MedianFilter(caminhoImagemSalt, caminhoImagemSaida, tamanhoJanela)
}