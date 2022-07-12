# Woodle

## Table of Contents
    
  - [About <a name = "about"></a>](#about-)
  - [Getting Started <a name = "getting_started"></a>](#getting-started-)
  - [Usage <a name = "usage"></a>](#usage-)

## About <a name = "about"></a>

Woodle é uma ideia de projeto utilizando Arduino para construir uma versão física do jogo Wordle (ou Termo, na versão brasileira).

## Getting Started <a name = "getting_started"></a>

O código foi originalmente desenvolvido no simulador online Wokwi, para visualizar as peças, montar as conexões e conferir o funcionamento dos componentes.

O simulador e a montagem dos componentes podem ser acessados por este link: https://wokwi.com/projects/323615837554475603

A última modificação no simulador foi feita em fevereiro de 2022.

## Usage <a name = "usage"></a>

Para executar o simulador, siga os seguintes passos:

- Na aba "Simulation", basta clicar no botão verde "Start the simulation". Pode utilizar o atalho <kbd>Ctrl</kbd> + <kbd>⏎ Enter</kbd> no Windows/Linux ou <kbd>⌘</kbd> <kbd>Enter</kbd> no Mac.
- A letras são inseridas utilizando os "push buttons" do simulador ou o próprio teclado, <kbd>⌫ Backspace</kbd> para apagar e <kbd>⏎ Enter</kbd> para finalizar a rodada ao preencher as 5 letras.
- "Leds" verdes, amarelos e cinzas indicam os erros/acertos de cada rodada:
  - _Verde, quando a letra estiver certa;_
  - _Amarelo, a letra existe na palavra, mas está na posição errada;_
  - _Cinza, a letra não está presente na palavra._
- Ao acertar a palavra, o jogo reinicia.

> **Nota:** A implementação está incompleta até o momento, então use apenas pela curiosidade. 

Por enquanto aceita qualquer sequência de caracteres como "palavra" válida e também a lista de onde o simulador escolhe a opção da vez é restrita às seguintes opções: _[amora, banan, borae, caman, dedos, dadin, dofua, erudi]_.

Ao finalizar as 6 rodadas sem acertar a palavra escolhida, o jogo "trava" e não reinicia sozinho.
