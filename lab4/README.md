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

