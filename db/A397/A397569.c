/*
 * A397569  a(1) = 0; for n >= 2, a(n) is the smallest number whose Hamming
 *          distance from a(n-1) is Fibonacci(n).
 * https://oeis.org/A397569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   bitlen_seq_t, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A397569: a(1)=0; for n≥2, a(n) is the smallest integer whose
   Hamming distance from a(n‑1) equals Fibonacci(n). */

#include <stdlib.h>          /* malloc, realloc */
#include "../utils/fib.h"    /* fib(idx_t) -> seq_t */
#include "../utils/bitlen_seq_t.h" /* bitlen_seq_t(seq_t) -> idx_t */

/* Count the number of 1‑bits in a seq_t value. */
static idx_t popcount_seq_t(seq_t x)
{
    idx_t cnt = 0;
    idx_t bl  = bitlen_seq_t(x);
    for (idx_t pos = bl; pos-- > 0; ) {
        if ( ((x >> pos) & (seq_t)1) != (seq_t)0 )
            ++cnt;
    }
    return cnt;
}

/* Compute the next term given a(n‑1)=prev and Fibonacci(n)=k. */
static seq_t compute_next(seq_t prev, seq_t k)
{
    idx_t ones = popcount_seq_t(prev);
    seq_t mask = (seq_t)0;

    if (k <= (seq_t)ones) {                     /* flip k most‑significant 1‑bits */
        idx_t need = (idx_t)k;
        idx_t bl   = bitlen_seq_t(prev);
        for (idx_t pos = bl; pos-- > 0; ) {
            if ( ((prev >> pos) & (seq_t)1) != (seq_t)0 ) {
                mask |= ((seq_t)1 << pos);
                if (!--need) break;
            }
        }
    } else {                                     /* flip all 1‑bits */
        mask = prev;                             /* all existing 1‑bits are flipped */
        idx_t need = (idx_t)(k - (seq_t)ones);
        idx_t pos = 0;
        while (need > 0) {
            if ( ((prev >> pos) & (seq_t)1) == (seq_t)0 ) {
                mask |= ((seq_t)1 << pos);
                --need;
            }
            ++pos;
        }
    }
    return prev ^ mask;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Cached values of the sequence. */
    static seq_t *cache     = NULL;   /* array of computed terms */
    static size_t cache_cap = 0;      /* allocated capacity (in terms) */
    static size_t cache_len = 0;      /* number of terms already computed */

    if (n == 1) return (seq_t)0;      /* a(1) = 0 */

    /* Ensure the cache has space for n terms. */
    if (n > (idx_t)cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure -> 0 as fallback */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the first term if needed. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];      /* a(i-1) */
        seq_t k    = fib(i);            /* Fibonacci(i) */
        cache[i - 1] = compute_next(prev, k);
    }
    cache_len = n;

    return cache[n - 1];
}
