# Arvore_Binaria_AVL
Implementação em C da Estrutura de Dados de Árvore Binária AVL. Essa árvore possui tamanho dinâmico ao mesmo tempo que garante a realização de buscas de maneira extremamente eficiente em todos os cenários possíveis. Ela foi conteúdo de diversas aulas de Algoritmo e Estrutura de Dados I ministrada pelo professor Lucioano Digiampietri. 

1. Para rodar: 
1.1 tenha instalado um compilador de C. Eu utilizei o MinGW com gcc.
1.2 no terminal digite gcc arvoreAVL.c -o arvoreAVL.exe
1.3 no terminal digite ./arvoreAVL.exe

2. Para testar:
2.1 não foi criada uma CLI para esse projeto
2.2 os testes ocorrem de maneira fixa pela main(), sinta-se a vontade para mudá-la e testá-la da forma que quiser

3. Explicação:
3.1 cada nó tem no máximo 2 filhos, o filho a esquerda deve ser sempre menor ou igual ao seu próprio valor, e o filho a direita deve ser maior que seu próprio valor
3.2 dessa forma, em uma busca, cada vez que o ponteiro "desce" a árvore ele corta pela metade as possibilidades de onde está a chave buscada (isso configura uma busca binária)
3.3 a diferença entre as árvores binárias de busca comuns e a AVL é que a AVL está sempre cuidando do balanceamento da árvore, garantindo uma distribuição equilibrada dos nós
3.4 assim, a complexidade de tempo de busca em árvore AVL é sempre, em todas as circunstâncias, theta(logn).

https://pt.wikipedia.org/wiki/%C3%81rvore_AVL
