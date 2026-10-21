/*
 * A294072  Number of noncube divisors of n^3.
 * https://oeis.org/A294072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑cube divisors of n³ = Π_i (3·e_i + 1) – Π_i
 *            (e_i + 1) where n = Π_i p_i^{e_i}
 * Range:     u8 n <= 359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of distinct prime factors of n and their
   exponents.  This helper is provided by the project. */
#include "../utils/factor_u64.h"
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety, not used */

    /* Factor n (64‑bit).  At most 64 distinct primes for uint64_t. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t D = (seq_t)1;   /* product of (3e+1)  */
    seq_t C = (seq_t)1;   /* product of (e+1)   */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        D *= (seq_t)(3 * e + 1);
        C *= (seq_t)(e + 1);
    }

    return D - C;
}
