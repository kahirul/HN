#include <string.h>
#include <time.h>
#include <ncurses.h>
#include "lib.h"
#include "ui.h"

const int pad_y = 1;
const int pad_x = 4;

const int hr_size = 80;

void init_curses()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
}

void print_normal(int y, const char *word)
{
    mvprintw(pad_y + y, pad_x, word);
}

void print_bold(int y, const char *word)
{
    attron(A_BOLD);
    mvprintw(pad_y + y, pad_x, word);
    attroff(A_BOLD);
}

char *sub_text(struct item item)
{
    char *buffer = malloc(sizeof(char) * 500);
    buffer[0] = '\0';

    int len = 0;

    char *score = i_to_s(item.score);
    strncat(buffer, score, strlen(score));
    len += strlen(score);

    char *filler_point = " points by ";
    strncat(buffer, filler_point, strlen(filler_point));
    len += strlen(filler_point);

    strncat(buffer, item.by, strlen(item.by));
    len += strlen(item.by);

    char *created = time_to_s(item.time, " %b %d, %H:%M | ");
    strncat(buffer, created, strlen(created));
    len += strlen(created);

    char *comment = i_to_s(item.descendants);
    strncat(buffer, comment, strlen(comment));
    len += strlen(comment);

    char *filler_comment = " comments";
    strncat(buffer, filler_comment, strlen(filler_comment));
    len += strlen(filler_comment);

    buffer[len] = '\0';

    return buffer;
}

void draw_app(struct item **top_stories, size_t topN)
{
    int in_char = 0;
    int h = 0;

    start_color();
    init_pair(1, COLOR_RED, COLOR_WHITE);

    attron(A_BOLD | COLOR_PAIR(1));
    mvprintw(pad_y + h++, pad_x, " Hacker News ");
    attroff(A_BOLD | COLOR_PAIR(1));

    mvhline(pad_y + h++, pad_x, '.', hr_size);

    for (size_t i = 0; i < topN; i++)
    {
        struct item *item = top_stories[i];
        print_bold(h++, item->title);
        print_normal(h++, sub_text(*item));
        h++;
    }

    print_bold(h++, "The Map That Made Los Angeles Make Sense (citylab.com)");
    print_normal(h++, "5 points by pseudolus 1 hour ago | discuss");

    print_normal(LINES - pad_y - 1, "Press 'Q' to quit");
    move(LINES - 1, pad_x);

    refresh();

    while (in_char != 'q')
    {
        in_char = getch();
    }

    endwin();
}
