enum fileLanguage_t {
  PT,
  EN
};

void beginStorage();
void closeFile();
void getLine(char* line, int index);
void getWord(int index, char word0[], char word1[]);
bool parseLine(char* line, char word0[], char word1[]);
void setLanguage(fileLanguage_t language);
int getWordsListLength();