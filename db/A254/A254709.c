/*
 * A254709  Indices of pentagonal numbers (A000326) which are also centered
 *          square numbers (A001844).
 * https://oeis.org/A254709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+14*a(n-2)-14*a(n-3)-a(n-4)+a(n-5). Initial values:
 *            a(1)=1, a(2)=2, a(3)=10, a(4)=25, a(5)=137
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;   /* number of elements stored (>=5 after init) */

/* ensure capacity for at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (cache_cap < needed)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1, this value is irrelevant */

    /* initialise first five terms if not already done */
    if (cache_len < 5)
    {
        ensure_capacity(5);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)25;
        cache[4] = (seq_t)137;
        cache_len = 5;
    }

    size_t need = (size_t)(n - 1);   /* zero‑based index of a(n) */

    if (cache_len <= need)          /* compute terms up to a(n) */
    {
        ensure_capacity(need + 1);
        for (size_t idx = cache_len; idx <= need; ++idx)
        {
            seq_t v1 = cache[idx - 1];
            seq_t v2 = cache[idx - 2];
            seq_t v3 = cache[idx - 3];
            seq_t v4 = cache[idx - 4];
            seq_t v5 = cache[idx - 5];
            seq_t pos = v1 + (seq_t)14 * v2 + v5;
            seq_t neg = (seq_t)14 * v3 + v4;
            cache[idx] = pos - neg;   /* guaranteed non‑negative */
        }
        cache_len = need + 1;
    }

    return cache[need];
}
