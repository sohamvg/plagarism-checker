#include "Word.h"
#include <stdlib.h>
#include <string.h>

Word *new_word(char *word_text)
{
    Word *word = malloc(sizeof(Word));
    strcpy(word->word_text, word_text);
    word->frequency = 1;
    return word;
}
