#ifndef _WORD_H_
#define _WORD_H_

#define MAX_WORD_SIZE 1024

typedef struct Word
{
    char word_text[MAX_WORD_SIZE];
    int frequency;
} Word;

Word *new_word(char *word_text);

#endif