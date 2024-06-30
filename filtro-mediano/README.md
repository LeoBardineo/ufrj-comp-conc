## Filtro de mediana paralelo

Forma de execução: go run main.go caminhoImagemEntrada tamanhoJanela nthreads

Arquivo principais:

- ``main.go``: contém o arquivo de execução principal, e as flags necessárias para executar sequencialmente, concorrente, aplicar o salt and pepper e comparar imagens
- ``medianfilter.go``: filtro mediano sequencial
- ``medianfilterparalelo.go``: filtro mediano paralelo
- ``equalimg.go``: compara duas imagens
- ``createlog.go``: cria log com a duração de cada execução na pasta logs
- ``log.sh``: ajuda para fazer os benchmarks