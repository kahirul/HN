#include "hn.h"

void init_curses();
void print_normal(int y, const char *word);
void print_bold(int y, const char *word);
void draw_app(struct item **top_stories, size_t topN);
char *sub_text(struct item item);
