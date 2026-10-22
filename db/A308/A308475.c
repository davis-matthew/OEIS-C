/*
 * A308475  a(1) = 1; a(n) = Sum_{k=1..n-1, gcd(n,k) = 1} binomial(n,k)*a(k).
 * https://oeis.org/A308475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = Sum_{k=1..n-1, gcd(n,k)=1} binomial(n,k) * a(k)
 *            (n >= 2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* dynamic array that stores a(0..computed) */
static size_t cap = 0;             /* current capacity of cache         */
static idx_t computed = 0;         /* highest n that has been computed  */

/* Euclidean algorithm for idx_t types (n,k <= 10000 in practice) */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Return a(n), computing all values up to n as needed. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* not defined, but harmless */

    /* Grow the cache if necessary. */
    if (n > computed) {
        size_t needed = (size_t)(n + 1);
        if (needed > cap) {
            size_t new_cap = cap ? cap * 2 : 1;
            while (new_cap < needed)
                new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        /* Compute missing terms sequentially. */
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
                continue;
            }

            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1;   /* C(i,0) */

            for (idx_t k = 1; k < i; ++k) {
                /* Update binom to C(i, k) using multiplicative recurrence. */
                binom = binom * (seq_t)(i - k + 1) / (seq_t)k;

                if (gcd_idx(i, k) == 1) {
                    sum += binom * cache[k];
                }
            }
            cache[i] = sum;
        }
        computed = n;
    }
    return cache[n];
}
