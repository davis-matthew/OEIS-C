/*
 * A292458  Numbers where 8 outnumbers any other digit.
 * https://oeis.org/A292458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A292458: numbers where the digit 8 occurs strictly more often than any other digit. */
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* Static cache of found numbers. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_cnt = 0;
static seq_t last_seen = 7;   /* value that will be incremented before first test */

/* Ensure that the cache can hold at least `need` elements. */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if (new_cap < need)
        new_cap = need;

    seq_t *new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();          /* allocation failure */

    /* Copy existing values. */
    for (size_t i = 0; i < cache_cnt; ++i)
        new_ptr[i] = cache[i];

    if (cache)
        free(cache);

    cache = new_ptr;
    cache_cap = new_cap;
}

/* Return the nth term (1‑based) of A292458. */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for 0, but return 0 safely */
        return 0;

    size_t target = (size_t)n;

    while (cache_cnt < target) {
        ++last_seen;  /* examine next integer */

        seq_t x = last_seen;
        unsigned int cnt[10] = {0};

        /* Count digits of x. */
        seq_t y = x;
        while (y > 0) {
            unsigned int d = (unsigned int)(y % 10);
            ++cnt[d];
            y = y / 10;
        }

        unsigned int max_other = 0;
        for (int d = 0; d < 10; ++d) {
            if (d == 8)
                continue;
            if (cnt[d] > max_other)
                max_other = cnt[d];
        }

        if (cnt[8] > max_other) {
            ensure_capacity(cache_cnt + 1);
            cache[cache_cnt++] = last_seen;
        }
    }

    return cache[target - 1];
}
