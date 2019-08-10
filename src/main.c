#include "hn.h"
#include "fetcher.h"

int main(int argc, char const *argv[])
{

    if (argc == 2)
    {
        fetch_item(argv[1], 0);
    }
    else
    {
        size_t topN = 3;
        struct item *top_stories[topN];
        fetch_top_stories(top_stories, topN);

        for (size_t i = 0; i < topN; i++)
        {
            printItem(*top_stories[i], true);
        }
    }

    return 0;
}
