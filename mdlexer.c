#include "mdlexer.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define INIT_CAP 256

void tokenarr_init(MDTokenArray *arr);
void tokenarr_append(MDTokenArray *arr, MDToken token);

void lexer_advance(MDLexer *lexer);
char lexer_current(MDLexer *lexer);
char lexer_peek(MDLexer *lexer);

MDLexer *mdlexer_new(char *src)
{
	MDLexer *lexer = malloc(sizeof(MDLexer));
	assert(lexer != NULL);

	lexer->src = strdup(src);
	lexer->srcLen = strlen(src);
	lexer->cursor = 0;

	return lexer;
}

void mdlexer_free(MDLexer *lexer)
{
	lexer->cursor = -1;
	free(lexer->src);
	free(lexer);
}

MDTokenArray *mdlexer_lex(MDLexer *lexer)
{
	MDTokenArray *tokens = malloc(sizeof(MDTokenArray));
	assert(tokens != NULL);
	tokens->cap = INIT_CAP;
	tokens->len = 0;
	tokenarr_init(tokens);

	while (lexer->cursor < lexer->srcLen) {
		MDToken token;
		// Headings
		if (lexer_current(lexer) == '#') {
			lexer_advance(lexer);

			// Space after hash is required
			if (lexer_current(lexer) != ' ')
				return NULL;

			lexer_advance(lexer);

			char text[256] = { 0 };
			int text_index = 0;
			while (lexer_current(lexer) != '\n') {
				text[text_index++] = lexer_current(lexer);
				lexer_advance(lexer);
			}

			text[text_index] = '\0';
			token.content = strdup(text);
			token.kind = MDTK_H1;
			tokenarr_append(tokens, token);

		// Paragraphs: assumes ascii only for now
		} else if (isalpha(lexer_current(lexer))) {
			char text[256] = { 0 };
			int text_index = 0;
			while (lexer_current(lexer) != '\0') {
				// If a true line break is found, also exit the loop
				if (lexer_peek(lexer) == '\n') {
					// We're advancing, so write the character it consumes
					text[text_index++] = lexer_current(lexer);
					lexer_advance(lexer);
					if (lexer_peek(lexer) == '\n')
						break;
				}

				// We're not breaking lines, so just write a space instead
				char curr = lexer_current(lexer);
				text[text_index++] = (curr == '\n')? ' ' : curr;
				lexer_advance(lexer);
			}

			text[text_index] = '\0';
			token.content = strdup(text);
			token.kind = MDTK_P;
			tokenarr_append(tokens, token);
		}

		lexer_advance(lexer);
	}

	return tokens;
}

void mdlexer_tokenarray_free(MDTokenArray *arr)
{
	arr->cap = 0;
	arr->len = 0;
	free(arr->items);
	free(arr);
}

void tokenarr_init(MDTokenArray *arr)
{
	arr->cap = INIT_CAP;
	arr->len = 0;
	arr->items = malloc(arr->cap*sizeof(MDToken));
	for (int i = 0; i < arr->cap; i++) {
		arr->items[i] = (MDToken){ .content = NULL, .kind = MDTK_NONE };
	}
}

void tokenarr_append(MDTokenArray *arr, MDToken token)
{
	if (arr->len >= arr->cap/2) {
		arr->cap *= 2;
		arr->items = realloc(arr->items, arr->cap*sizeof(MDToken));
	}
	arr->items[arr->len++] = token;
}

void lexer_advance(MDLexer *lexer)
{
	if (lexer->cursor < lexer->srcLen)
		++lexer->cursor;
}

char lexer_current(MDLexer *lexer)
{
	return lexer->src[lexer->cursor];
}

char lexer_peek(MDLexer *lexer)
{
	if (lexer->cursor + 1 < lexer->srcLen)
		return lexer->src[lexer->cursor + 1];

	return lexer->src[lexer->cursor];
}
