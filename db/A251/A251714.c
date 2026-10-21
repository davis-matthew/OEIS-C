/*
 * A251714  7-step Fibonacci sequence starting with (0,1,0,0,0,0,0).
 * https://oeis.org/A251714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+7) = a(n)+a(n+1)+a(n+2)+a(n+3)+a(n+4)+a(n+5)+a(n+6) with
 *            initial values a(0)=0, a(1)=1, a(2)=a(3)=a(4)=a(5)=a(6)=0
 * Range:     u8 n <= 15, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t last_computed = 0;

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cap(idx_t n)
{
    if ((size_t)n < cap)
        return;
    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed */
    cache = tmp;
    cap   = new_cap;
}

/* Initialise the first seven terms; called only once. */
static void init_base(idx_t n)
{
    if (cap < 7)
        ensure_cap(6);

    cache[0] = 0;
    cache[1] = 1;
    for (idx_t i = 2; i <= 6; ++i)
        cache[i] = 0;

    if (n <= 6)       /* if caller only needs a term < 7 we are done */
        last_computed = n;
    else
        last_computed = 6;
}

seq_t a(idx_t n)
{
    /* Handle terms 0..6 directly after initialising the cache. */
    if (cache == NULL)          /* first call */
        init_base(n);
    else if (n <= 6) {          /* cache already filled */
        return cache[n];
    }

    /* Extend the cache up to n if necessary. */
    if ((idx_t)last_computed < n) {
        ensure_cap(n);
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i <= 6) {      /* base terms – already set but keep for consistency */
                cache[i] = (i == 1) ? 1 : 0;
            } else {
                cache[i] =
                    cache[i - 1] + cache[i - 2] + cache[i - 3] +
                    cache[i - 4] + cache[i - 5] + cache[i - 6] + cache[i - 7];
            }
        }
        last_computed = n;
    }

    return cache[n];
}
