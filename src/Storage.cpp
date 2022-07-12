#include <SPI.h>
#include <SdFat.h>

#include "Storage.h"

#include "SerialDebug.h"

#define SPI_SPEED SD_SCK_MHZ(4)
#define CS_PIN 53

#define WORDS_LIST 0
#define WORDS_SELECT 1

struct sFile_t {
  fileLanguage_t lang;
  char* list;
  char* select;
};

struct sIndex_t {
  fileLanguage_t lang;
  int totalWords;
  int charIndex[26];
};

const sFile_t langFile[] = {
  {PT, "pt_list.txt", "pt_slct.txt"},
  {EN, "en_list.txt", "en_slct.txt"},
};

sIndex_t wordsIndex[] = {
  {PT, 0, {0}},
  {EN, 0, {0}},
};

SdFat sd;
File file;

fileLanguage_t currentLanguage = PT;

char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
// int8_t firstCharIndex[26] = {0};

bool openFile(uint8_t fileType) {
  char* path;

  switch (fileType) {
    case WORDS_LIST:
      path = langFile[currentLanguage].list;
      break;
    case WORDS_SELECT:
      path = langFile[currentLanguage].select;
      break;
  }

  if (sd.exists(path)) {
    return file.open(path);
  }

  PRINTS("Err 'openFile'\n");

  return false;
}

void closeFile() {
  file.close();
}

void moveCursorToLine(int line) {
  // Cada linha possui 12 caracteres, contando com o '\n'
  file.seekSet(line * 12) ;
}

// void openFile() {
//   openFile(WORDS_LIST);

//   if (file) {
//     // read from the file until there's nothing else in it:
//     while (file.available()) {
//       byte b = file.read();
//     }

//     closeFile();
//   } else {
//     // if the file didn't open, print an error:
//     PRINTS(" err open file\n");
//   }
// }

void processWordsIndex() {
  long int t1 = millis();

  byte originalLanguage = currentLanguage;
  uint8_t sizeArray = sizeof(langFile) / sizeof(*langFile);

  PRINTS("\nwords index\n");

  for (uint8_t i = 0; i < sizeArray; i++) {
    currentLanguage = langFile[i].lang;

    if (openFile(WORDS_LIST)) {
      bool newLine = true;

      int wordsCounter = 0;
      byte ascii = 0;
      byte currentChar = 0;
      int indexCounter = 0;
      int position = 0;

      while (file.available()) {
        ascii = file.read();

        if (newLine) {
          newLine = false;

          if (ascii > currentChar) {
            wordsIndex[currentLanguage].charIndex[indexCounter++] = position;
            currentChar = ascii;
          }
        }

        // if (ascii >= 65 && ascii <= 93) {
        //   ascii += 159;
        // }

        if (ascii == 10) { // count CR in lines
          wordsCounter++;
          newLine = true;
        }

        position++;
      }

      wordsIndex[currentLanguage].totalWords = wordsCounter;

      PRINT("lang: ", currentLanguage);
      PRINT(" / words: ", wordsCounter);
      PRINTS("\n");

      closeFile();
    }
  }

  currentLanguage = originalLanguage;

  long int t2 = millis();

  PRINT("time index: ", t2 - t1);
  PRINTS(" ms\n");
}

int getWordsListLength() {
  return wordsIndex[currentLanguage].totalWords;
}

void getLine(char* line, int index) {
  openFile(WORDS_LIST);

  if (file) {
    int8_t position = 0;

    moveCursorToLine(index);

    while (file.available() && file.peek() != 10) {
      byte ascii = file.read();

      if (ascii >= 65 && ascii <= 93) {
        // Corrigindo a substituição das letras acentuadas
        ascii += 159;
      }

      line[position++] = (char)ascii;
    }

    line[position] = '\0';

    closeFile();
  } else {
    PRINTS("Err 'getLine'\n");
  }
}

// Para separar as palavras da linha
bool parseLine(char* line, char* word0, char* word1) {
  // Set strtok start of line.
  char* str = strtok(line, ",");

  if (!str) return false;

  strcpy(word0, str);

  // Subsequent calls to strtok expects a null pointer.
  str = strtok(nullptr, ",");

  if (!str) return false;

  strcpy(word1, str);

  // Check for extra fields.
  return strtok(nullptr, ",") == nullptr;
}

void getWord(int index, char* word0, char* word1) {
  char line[12];

  getLine(line, index);

  PRINT("line: ", line);
  PRINTS("\n");

  if (!parseLine(line, word0, word1)) {
    PRINTS("Err 'parseLine'\n");
  }
}

void setLanguage(fileLanguage_t languageSelected) {
  currentLanguage = languageSelected;

  // if (currentLanguage == PT) {
  //   listFileName = PT_LIST;
  //   selectionFileName = PT_SELECT;
  // } else {
  //   listFileName = EN_LIST;
  //   selectionFileName = EN_SELECT;
  // }
}

void beginStorage() {
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  PRINTS("Initializing SD card...");

  if (!sd.begin(CS_PIN, SPI_SPEED)) {
    PRINTS(" failed.\n");
    while (1);
  }

  PRINTS(" done.\n");

  processWordsIndex();
}