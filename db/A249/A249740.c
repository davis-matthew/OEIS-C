/*
 * A249740  The largest prime whose square divides n, 1 if n is squarefree.
 * https://oeis.org/A249740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest prime whose square divides n; returns 1 if n is squarefree. */
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* Storage for prime factors and their exponents. 64 entries are more than enough for 64‑bit numbers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factor n using the provided helper. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t largest = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] >= 2) {
            seq_t p = (seq_t)primes[i];
            if (p > largest)
                largest = p;
        }
    }
    return largest;
}
