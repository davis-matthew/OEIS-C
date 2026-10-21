/*
 * A247344  a(n) = 1 for n <= 4; a(n) = 25*a(n-1) - 200*a(n-2) + 800*a(n-3) -
 *          1600*a(n-4) + 1280*a(n-5) otherwise.
 * https://oeis.org/A247344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n <= 4; a(n) = 25*a(n-1) - 200*a(n-2) + 800*a(n-3)
 *            - 1600*a(n-4) + 1280*a(n-5) for n > 4 Offset: 0 (a(0) is the
 *            first term)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* computed terms */
static size_t cap  = 0;          /* allocated capacity */
static size_t len  = 0;          /* number of terms currently stored (>=0) */

/* Ensure that values a(0) … a(n) are present in cache. */
static void ensure_cache(idx_t n)
{
    /* already computed? */
    if (n + 1 <= len)
        return;

    /* allocate enough space for up to n */
    size_t new_len = (size_t)n + 1;
    if (new_len > cap) {
        size_t new_cap = (cap == 0) ? 8 : cap * 2;
        while (new_cap < new_len)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);                  /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise the first 5 terms (a(0) … a(4) = 1) if not already done */
    if (len == 0) {
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 1;
        len     = 5;
    }

    /* compute missing terms from len up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        /* recurrence valid only for i >= 5, but loop starts at len >=5 */
        if (i >= 5) {
            seq_t val = 25 * cache[i - 1];
            val -= 200 * cache[i - 2];
            val += 800 * cache[i - 3];
            val -= 1600 * cache[i - 4];
            val += 1280 * cache[i - 5];
            cache[i] = val;
        }
    }

    /* update stored length if we extended it */
    if (new_len > len)
        len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
