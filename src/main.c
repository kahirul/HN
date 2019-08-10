#include "hn.h"
#include "fetcher.h"
#include "ui.h"

int main(int argc, char const *argv[])
{

    if (argc == 2)
    {
        fetch_item(argv[1], 0);
    }
    else
    {
        size_t topN = 15;
        struct item *top_stories[topN];
        fetch_top_stories(top_stories, topN);

        init_curses();
        draw_app(top_stories, topN);
    }

    return 0;
}
