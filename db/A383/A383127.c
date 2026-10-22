/*
 * A383127  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (205*s + 29573) mod 139968 when
 *          started at 1.
 * https://oeis.org/A383127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (205*a(n-1) + 29573) mod 139968, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Linear congruential generator with period 139968 */
seq_t a(idx_t n)
{
    /* Lazy initialization of the period table */
    static seq_t *cache = NULL;
    const idx_t P = 139968ULL;          /* period length */
    if (cache == NULL) {
        cache = (seq_t *)malloc(P * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* out‑of‑memory fallback */
        cache[0] = (seq_t)1;            /* a(1) = 1 */
        for (idx_t i = 1; i < P; ++i) {
            long long t = (long long)cache[i-1] * 205LL + 29573LL;
            cache[i] = (seq_t)(t % 139968LL);
        }
    }

    /* n is 1‑based; wrap around the period */
    idx_t idx = (n - 1) % P;
    return cache[idx];
}
