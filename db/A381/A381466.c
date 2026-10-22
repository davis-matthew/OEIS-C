/*
 * A381466  a(0) = 4; for n > 0, a(n) = a(n-1) + n if g = 1, otherwise a(n) =
 *          n/g, where g = gcd(a(n-1), n).
 * https://oeis.org/A381466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0)=4; for n>0: a(n)=a(n-1)+n if gcd(a(n-1),n)=1 else
 *            a(n)=n/gcd(a(n-1),n)
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;      /* array of computed values */
static idx_t cache_len = 0;      /* number of elements stored  (next free index) */
static size_t cache_cap = 0;     /* allocated capacity (in elements) */

static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    if (new_cap < (size_t)needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed; terminate to avoid undefined behavior */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Handle the first term explicitly */
    if (n == 0)
        return (seq_t)4;

    /* Initialize the cache with the initial term if it hasn't been done yet */
    if (cache_len == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)4;
        cache_len = 1;
    }

    /* Extend the cache up to index n if necessary */
    if (n >= cache_len) {
        ensure_capacity(n + 1);  /* allocate space for indices 0..n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t cn  = (seq_t)i;            /* cast index to seq_t */
            seq_t g   = gcd(prev, cn);      /* use the provided gcd() function */
            seq_t val;
            if (g == (seq_t)1)
                val = prev + cn;
            else
                val = cn / g;
            cache[i] = val;
        }
        cache_len = n + 1;   /* update length to include n */
    }
    return cache[n];
}
