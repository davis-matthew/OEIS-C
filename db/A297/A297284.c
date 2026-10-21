/*
 * A297284  Numbers whose base-14 digits have greater up-variation than
 *          down-variation; see Comments.
 * https://oeis.org/A297284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   by scanning integers and checking base‑14 up‑variation >
 *            down‑variation
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;
static seq_t last_checked = 0;

static void grow_cache(size_t required)
{
    if (required <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < required)
        new_cap *= 2;
    seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (cache)
    {
        memcpy(tmp, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset is 1, value for n=0 is irrelevant */

    while (cache_len < n)
    {
        seq_t val = last_checked + 1;
        last_checked = val;

        unsigned char digits[256];
        size_t len = 0;

        seq_t tmp = val;
        if (tmp == 0)
        {
            digits[0] = 0;
            len = 1;
        }
        else
        {
            while (tmp > 0)
            {
                digits[len++] = (unsigned char)(tmp % 14);
                tmp /= 14;
            }
            /* digits now hold least‑significant first, which is
               the correct order for the variation calculation. */
        }

        seq_t uv = 0, dv = 0;
        for (size_t i = 0; i + 1 < len; ++i)
        {
            unsigned char d0 = digits[i];
            unsigned char d1 = digits[i + 1];
            if (d1 < d0)
                uv += (seq_t)(d0 - d1);
            else if (d1 > d0)
                dv += (seq_t)(d1 - d0);
        }

        if (uv > dv)
        {
            grow_cache(cache_len + 1);
            cache[cache_len++] = val;
        }
    }

    return cache[n - 1];
}
