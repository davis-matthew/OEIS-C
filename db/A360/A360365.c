/*
 * A360365  a(n) = sum of the products of the digits of the first n positive
 *          multiples of 3.
 * https://oeis.org/A360365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{k=1..n} product of decimal digits of (3*k)
 * Range:     u8 n <= 21, u16 n <= 765, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the sum of the digit products of the first n
   positive multiples of 3.  Results smaller than the size of seq_t are
   exact; larger values overflow modulo 2^N as per the type. */

#include <stddef.h>
#include <stdlib.h>

/* Compute the product of decimal digits of x.
   x is a non‑negative idx_t, which fits in the machine word. */
static inline seq_t digit_prod(idx_t x)
{
    seq_t prod = 1;
    while (x > 0) {
        prod *= (seq_t)(x % 10);
        x /= 10;
    }
    return prod;
}

seq_t a(idx_t n)
{
    /* Memoisation: cumulative sums up to the largest cached n. */
    static seq_t *cache = NULL;       /* cache[0] == a(1) */
    static idx_t  cached_n = 0;       /* largest n that has been cached */

    /* Offset starts at 1; guard against n == 0. */
    if (n == 0) return 0;

    if (n > cached_n) {
        /* Allocate or grow the cache to cover the required n. */
        size_t new_size = (size_t)n;                 /* n <= 10000 → safe */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) return 0;                       /* allocation failed */

        /* Compute new entries. */
        for (idx_t k = cached_n + 1; k <= n; ++k) {
            idx_t mult = k * (idx_t)3;              /* 3*k */
            seq_t prod = digit_prod(mult);

            if (k == 1)
                cache[0] = prod;
            else
                cache[k - 1] = cache[k - 2] + prod;
        }
        cached_n = n;
    }

    return cache[n - 1];
}
