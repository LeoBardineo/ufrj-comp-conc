package createlog

import(
	"strconv"
	"os"
	"log"
)

func CreateLog(duracao time.Duration, logFile string) {
    duracaoStr := strconv.FormatFloat(duracao.Seconds(), 'f', 3, 64)
	
	arquivo, err := os.OpenFile("logs/" + logFile + ".out", os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0777)
	if err != nil {
		log.Fatal("Erro ao abrir ou criar o arquivo:", err)
	}
	defer arquivo.Close()
	
	if _, err := arquivo.WriteString(duracaoStr + "\n"); err != nil {
		log.Fatal("Erro ao criar arquivo", err)
	}
}