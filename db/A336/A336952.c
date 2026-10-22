/*
 * A336952  E.g.f.: 1 / (1 - x * exp(4*x)).
 * https://oeis.org/A336952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = Σ_{k=1..n} binomial(n,k) * k * 4^(k‑1) * a(n‑k)
 *            for n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Computes the nth term of OEIS sequence A336952. */
seq_t a(idx_t n)
{
    /* Cache for previously computed values. */
    static seq_t *cache = NULL;   /* array of computed a(i) */
    static size_t   cap   = 0;    /* allocated capacity of cache */
    static size_t   sz    = 0;    /* number of values currently stored (next free index) */

    /* Ensure cache has room for indices 0 … n. */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t new_cap = cap ? cap << 1 : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocator failure: abort */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n. */
    for (size_t i = sz; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;          /* a(0) = 1 */
            continue;
        }

        seq_t sum   = 0;
        seq_t binom = (seq_t)i;          /* binomial(i,1) */
        seq_t pow4  = (seq_t)1;          /* 4^(k-1), initially k=1 */

        for (idx_t k = 1; k <= (idx_t)i; ++k) {
            seq_t term = binom * (seq_t)k * pow4 * cache[i - (size_t)k];
            sum += term;

            /* Update binom to C(i,k+1), avoiding overflow by dividing after multiplication.
               binom = binom * (i - k) / (k + 1);   */
            binom = binom * (seq_t)(i - (size_t)k) / (seq_t)(k + 1);

            pow4 *= (seq_t)4;   /* increment power of 4 */
        }
        cache[i] = sum;          /* a(i) = sum */
    }
    sz = n + 1;

    return cache[(size_t)n];
}
