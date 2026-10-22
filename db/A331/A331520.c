/*
 * A331520  a(0) = a(1) = 1; a(n+2) = Sum_{k=0..n} (binomial(n,k) mod 2) *
 *          a(k).
 * https://oeis.org/A331520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1; a(n) = Sum_{k=0..n-2} (C(n-2,k) mod 2) * a(k)
 * Range:     u8 n <= 16, u16 n <= 36, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;   /* memoized values */
static idx_t size   = 0;      /* number of values stored (next free index) */
static idx_t cap    = 0;      /* capacity of cache */

/* Grow the cache so it can hold indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if (cache == NULL) {
        cap   = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        /* initialize first two terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        size    = 2;
    }
    if (n + 1 <= cap) return;
    idx_t newcap = cap ? cap * 2 : 4;
    while (newcap < n + 1) newcap <<= 1;
    cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
    if (cache == NULL) { /* allocation failed; terminate program */
        abort();
    }
    cap = newcap;
}

/* Return a(n) for any n >= 0.  The function memoizes the sequence
   so repeated calls in any order are efficient. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)1;  /* a(0) = a(1) = 1 */

    ensure_cache_size(n);

    /* Compute missing values up to n */
    for (idx_t m = size; m <= n; ++m) {
        if (m < 2) {
            cache[m] = (seq_t)1;
        } else {
            /* a(m) = sum_{k subset of m-2} a(k)      */
            idx_t bits = m - 2;
            seq_t sum  = 0;
            idx_t sub  = bits;
            for (;;) {
                sum += cache[sub];
                if (sub == 0) break;
                sub = (sub - 1) & bits;
            }
            cache[m] = sum;
        }
    }
    size = n + 1;
    return cache[n];
}
