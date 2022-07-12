#include "Display.h"
#include "Storage.h"
#include "Keyboard.h";
#include "Light.h";

#include "SerialDebug.h"

#define NUM_COLUMNS 5

char currentWord[6]; // Palavra selecionada para o jogo
char displayWord[6]; // Palavra sem acentos para comparação
uint8_t trialNumber = 0; // Número da tentativa na rodada (0..5)
uint8_t rightLetters = 0; // Qtd. de letras corretas na tentativa

uint8_t cursorPosition = 0; // Posição do cursor para exibir a letra digitada
char playerWord[6]; // Palavra da tentativa do jogador
char displayLine[6]; // Texto que será enviado para tela

const char keys[28] = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '-', '='};

// right / place / wrong
char status[6][5] = {
  {'\0', '\0', '\0', '\0', '\0'},
  {'\0', '\0', '\0', '\0', '\0'},
  {'\0', '\0', '\0', '\0', '\0'},
  {'\0', '\0', '\0', '\0', '\0'},
  {'\0', '\0', '\0', '\0', '\0'},
  {'\0', '\0', '\0', '\0', '\0'}
};

void newGame() {
  resetData();
  cleanDisplay();

  int maxValue = getWordsListLength();
  int randomIndex = random(maxValue);

  PRINT("random: ", randomIndex);
  PRINTS("\n");

  getWord(randomIndex, displayWord, currentWord);

  PRINT("word: ", currentWord);
  PRINTS("\n");
}

void resetData() {
  trialNumber = 0;
  rightLetters = 0;
  cursorPosition = 0;

  for (uint8_t i = 0; i < 6; i++) {
    for (uint8_t j = 0; j < 5; j++) {
      status[i][j] = '\0';
    }
  }

  memset(playerWord, '\0', sizeof(playerWord));
  memset(displayWord, '\0', sizeof(displayWord));
  memset(currentWord, '\0', sizeof(currentWord));
  memset(displayLine, '\0', sizeof(displayLine));
}

bool checkWord() {
  char controlWord[6];
  strcpy(controlWord, displayWord);

  PRINT("player ", playerWord);
  PRINT("\ndisplay ", displayWord);
  PRINT("\ncontrol ", controlWord);
  PRINTS("\n");

  rightLetters = 0;

  // Marca letras certas, nas posições certas
  for (uint8_t i = 0; i < 5; i++) {
    if (playerWord[i] == controlWord[i]) {
      status[trialNumber][i] = 'O';
      controlWord[i] = '-';
      rightLetters++;
    }
  }

  PRINT("player ", playerWord);
  PRINT("\ndisplay ", displayWord);
  PRINT("\ncontrol ", controlWord);
  PRINTS("\n");

  if (rightLetters == 5) {
    return true;
  }

  // Marca letras certas, nas posições erradas
  for (uint8_t i = 0; i < 5; i++) {
    for (uint8_t j = 0; j < 5; j++) {
      if (status[trialNumber][i] == '\0' && playerWord[i] == controlWord[j]) {
        status[trialNumber][i] = '?';
        controlWord[j] = '-';
      }
    }
  }

  PRINT("player ", playerWord);
  PRINT("\ndisplay ", displayWord);
  PRINT("\ncontrol ", controlWord);
  PRINTS("\n");

  // Marca as letras erradas
  for (uint8_t i = 0; i < 5; i++) {
    if (status[trialNumber][i] == '\0') {
      status[trialNumber][i] = 'X';
      controlWord[i] = '-';
    }
  }

  return false;
}

void printStatus() {
  for (uint8_t i = 0; i < 5; i++) {
    PRINT("[ ", status[trialNumber][i]);
    PRINTS(" ] ");

    switch (status[trialNumber][i]) {
      case 'O':
        lightOn(trialNumber * NUM_COLUMNS + i, green);
        break;
      case '?':
        lightOn(trialNumber * NUM_COLUMNS + i, yellow);
        break;
      case 'X':
        lightOn(trialNumber * NUM_COLUMNS + i, black);
        break;
    }

    delay(100);    
  }

  PRINTS("\n");
}

void submitRound() {
  if (!checkWord()) {
    printStatus();

    if (trialNumber == 5) {
      // Perdeu, mané!
    } else {
      trialNumber++;
      cursorPosition = 0;

      memset(playerWord, '\0', sizeof(playerWord));
      // memset(displayLine, '\0', sizeof(displayLine));

      displayLine[cursorPosition] = '^';
      displayLine[cursorPosition + 1] = '\0';

      changeDisplayData(displayLine, trialNumber);
    }
  } else {
    // Êêê, ganhou!!!
    printStatus();

    newGame();
  }


}

void onKeyPressed(uint8_t index) {
  switch (index) {
    case 26: // backspace
      if (cursorPosition > 0) {
        playerWord[cursorPosition] = '\0';
        displayLine[cursorPosition] = '\0';

        cursorPosition--;

        displayLine[cursorPosition] = '^';
        // displayLine[cursorPosition + 1] = '\0';

        changeDisplayData(displayLine, trialNumber);
      }

      break;
    case 27: // enter
      if (cursorPosition == 5) {
        submitRound();
      } else {
        PRINTS("not enough letters / só palavras com 5 letras\n");
      }
      break;
    default:
      if (cursorPosition <= 4) {
        playerWord[cursorPosition] = keys[index];
        displayLine[cursorPosition] = keys[index];

        cursorPosition++;

        if (cursorPosition == 5) {
          playerWord[cursorPosition] = '\0';
          displayLine[cursorPosition] = '\0';
        } else if (cursorPosition < 5) {
          displayLine[cursorPosition] = '^';
          displayLine[cursorPosition + 1] = '\0';
        }

        changeDisplayData(displayLine, trialNumber);
        PRINT("line: ", displayLine);
        PRINTS("\n");
      }
  }
}

void doUI() {
  if (keyboardStatusChanged()) {
    for (uint8_t i = 0; i < countKeys(); i++) {
      if (keyStatus(i) == 1) {
        PRINT("pressed: ", i);
        PRINTS("\n");
        onKeyPressed(i);
      }
    }
  }

  delay(1);
}

void setup(void)
{
  Serial.begin(115200);
  randomSeed(analogRead(0));

  beginStorage();
  beginDisplay();
  beginKeyboard();
  beginLight();

  setLanguage(PT);

  newGame();
}

void loop(void)
{
  doUI();  
  updateDisplay();
}

// String literals are copied into SRAM and byte away
// (pun intended) at that memory space.
// The F() macro prevents them from being copied into
// SRAM memory space. You can also get an idea of the
// amount of space you have left by using the following function
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}