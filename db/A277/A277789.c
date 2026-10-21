/*
 * A277789  a(n) = Sum_{k=0..n} (-1)^k*floor((1 + sqrt(2))^k).
 * https://oeis.org/A277789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * floor((1 + sqrt(2))^k). Recurrence:
 *            a(n) = -a(n-1) + 4*a(n-2) - 3*a(n-4) + a(n-5) (n >= 5). Base
 *            values: a(0)=1, a(1)=-1, a(2)=4, a(3)=-10, a(4)=23
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 24, i64 n <= 49, i128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed values   */
static size_t cache_len = 0;     /* number of values in cache  */
static size_t cache_cap = 0;     /* capacity of cache array    */

static void ensure_cache(idx_t n)
{
    /* Grow cache until it can hold index n (inclusive) */
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n + 1)
        new_cap *= 2;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

static void init_base()
{
    /* Initialise base cases a(0) .. a(4) */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)-1;
    cache[2] = (seq_t)4;
    cache[3] = (seq_t)-10;
    cache[4] = (seq_t)23;
    cache_len = 5; /* next index to compute */
}

seq_t a(idx_t n)
{
    ensure_cache(n);

    if (cache_len == 0) {
        if (n < 5) {
            /* n < 5: just fill needed part */
            for (idx_t i = 0; i <= n; ++i) {
                if (i == 0) cache[i] = (seq_t)1;
                else if (i == 1) cache[i] = (seq_t)-1;
                else if (i == 2) cache[i] = (seq_t)4;
                else if (i == 3) cache[i] = (seq_t)-10;
                else if (i == 4) cache[i] = (seq_t)23;
            }
            cache_len = n + 1;
            return cache[n];
        } else {
            init_base();
        }
    }

    /* Compute values up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i < 5) {
            /* Should not happen because base already set, but guard anyway */
            if (i == 0) cache[i] = (seq_t)1;
            else if (i == 1) cache[i] = (seq_t)-1;
            else if (i == 2) cache[i] = (seq_t)4;
            else if (i == 3) cache[i] = (seq_t)-10;
            else if (i == 4) cache[i] = (seq_t)23;
        } else {
            seq_t val = -cache[i - 1];
            val += (seq_t)4 * cache[i - 2];
            val -= (seq_t)3 * cache[i - 4];
            val += cache[i - 5];
            cache[i] = val;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
