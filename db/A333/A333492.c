/*
 * A333492  Position of first appearance of n in A271410 (LCM of binary
 *          indices).
 * https://oeis.org/A333492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ 2^(p^e - 1) over all prime powers p^e dividing n (with
 *            n ≥ 1; a(1) = 1). Does not rely on any precomputed tables
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    idx_t m = n;
    seq_t result = 0;
    idx_t p = 2;

    /* Trial‑division factorisation of n.  For each distinct prime p we
       collect the prime power p^e (product of all copies of p). */
    while (p * p <= m) {
        if (m % p == 0) {
            idx_t prime_power = 1;
            while (m % p == 0) {
                m /= p;
                prime_power *= p;           /* prime_power = p^e  */
            }
            result += pow2(prime_power - 1); /* 2^(p^e‑1) */
        }
        /* Advance to next candidate divisor: 2 → 3 → 5 → 7 …  */
        p += (p == 2 ? 1 : 2);
    }

    /* If a prime factor > sqrt(n) remains, it is itself a prime. */
    if (m > 1) {
        result += pow2(m - 1);                /* 2^(m‑1)            */
    }

    return result;
}
