## Respostas do laboratório 4

### Atividade 1

3. Os valores impressos foram sempre o valor esperado? Se não, explique por que isso aconteceu?

    Não, pois como as threads estão acessando uma mesma variável com memória compartilhada, lendo e alterando ela sem uma thread esperar a outra, elas podem acabar adicionando à um valor atrasado.
    
    Isso ocorre porque a adição feita na linha 19 é transformada em três instruções de máquina, que consiste em ler a soma colocando em um registrador, adicionar 1 ao registrador, e colocar o resultado na variável.
    
    Como as threads estão sendo executadas juntas, por vezes pode acontecer das duas threads lerem o valor atual de soma antes mesmo de ter a chance ser atualizado, ou então uma thread ler o valor de soma, e antes mesmo de somar, outra thread somar e atualizar o valor de soma, fazendo com que a thread anterior atualize um valor anterior. Tudo isso resulta à diversas inconsistências e imprevisibilidade à cerca do valor de soma.

4. É possível que a variável soma termine com valor acima de 200000? Por quê?

    Não, pois para isso seria necessário provocar adições adicionais, o que não é possível ocorrer já que ambas as threads operam um máximo de 100000 somas por conta do loop que as threads executam.

### Atividade 2

2. Os valores impressos foram sempre o valor esperado? Por quê?

    Sim, pois o lock foi implementado de forma à impedir que uma thread leia e altere o valor de soma enquanto outra thread já está realizando isso, fazendo com que a thread espere a outra terminar para então ler o valor atualizado, somar e alterar a variável.

### Atividade 3

Feito utilizando o computador do lab, que tem i5 13500 como CPU e 16 GB RAM.

1 threads, N = 10^6, Média: 0.090400, Aceleração: 1.000000000, Eficiência: 0.250000000
2 threads, N = 10^6, Média: 0.063060, Aceleração: 1.433555344, Eficiência: 0.358388836
4 threads, N = 10^6, Média: 0.038400, Aceleração: 2.354166667, Eficiência: 0.588541667
8 threads, N = 10^6, Média: 0.025530, Aceleração: 3.540932237, Eficiência: 0.885233060

1 threads, N = 10^7, Média: 1.937330, Aceleração: 1.000000000, Eficiência: 0.250000000
2 threads, N = 10^7, Média: 1.229820, Aceleração: 1.575295571, Eficiência: 0.393823892
4 threads, N = 10^7, Média: 0.668250, Aceleração: 2.899109614, Eficiência: 0.724777403
8 threads, N = 10^7, Média: 0.418180, Aceleração: 4.632765794, Eficiência: 1.158191448