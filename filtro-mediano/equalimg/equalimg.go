package equalimg

import (
	"fmt"
	"image"
	"os"
)

func abrirImagem(filePath string) (image.Image, error) {
	file, err := os.Open(filePath)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	img, _, err := image.Decode(file)
	if err != nil {
		return nil, err
	}

	return img, nil
}

func CompararImagens(caminhoImagem1, caminhoImagem2 string) {
	img1, err := abrirImagem(caminhoImagem1)
	if err != nil {
		fmt.Printf("Erro ao abrir a primeira imagem: %v\n", err)
		return
	}

	img2, err := abrirImagem(caminhoImagem2)
	if err != nil {
		fmt.Printf("Erro ao abrir a segunda imagem: %v\n", err)
		return
	}

	bounds1 := img1.Bounds()
	bounds2 := img2.Bounds()
	
	if !bounds1.Eq(bounds2) {
		fmt.Printf("ERRO: As imagens \"%s\" e \"%s\" não são iguais.\n", caminhoImagem1, caminhoImagem2)
		return
	}
	
	imagensIguais := true
	for y := bounds1.Min.Y; y < bounds1.Max.Y; y++ {
		for x := bounds1.Min.X; x < bounds1.Max.X; x++ {
			if img1.At(x, y) != img2.At(x, y) {
				imagensIguais = false
				break
			}
		}
	}
	
	if !imagensIguais {
		fmt.Printf("ERRO: As imagens \"%s\" e \"%s\" não são iguais.\n", caminhoImagem1, caminhoImagem2)
		return
	}
	fmt.Printf("O TESTE PASSOU: As imagens \"%s\" e \"%s\" são iguais.\n", caminhoImagem1, caminhoImagem2)
}
