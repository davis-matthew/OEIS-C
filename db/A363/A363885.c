/*
 * A363885  Number of divisors of 7*n-3 of form 7*k+2.
 * https://oeis.org/A363885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of 7*n-3 that are ≡ 2 (mod 7)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Count the divisors of m = 7*n-3 that are congruent to 2 modulo 7. */
seq_t a(idx_t n)
{
    if (n == 0) {          /* caller is expected to use offset 1, but guard */
        return (seq_t)0;
    }

    /* Compute m = 7*n - 3.  It is safe because n < 2^64/7 for usual test ranges. */
    uint64_t m = (uint64_t)n * 7ULL - 3ULL;

    /* Remove all factors of 7 since any divisor containing 7 is ≡ 0 (mod 7). */
    uint64_t r = m;
    while ((r % 7ULL) == 0ULL) {
        r /= 7ULL;
    }

    /* Special case: r == 1 means the only divisor left is 1, which is not ≡ 2 (mod 7). */
    if (r == 1ULL) {
        return (seq_t)0;
    }

    /* Factor r.  Use small static arrays since r ≤ 7*n‑3 is modest in the test range. */
    static uint64_t primes[32];
    static uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(r, primes, exps, &cnt);

    /* Dynamic programming over residues modulo 7. */
    int counts[7]  = {0};
    int newc[7];
    counts[1 % 7] = 1;          /* divisor 1 */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        int      e   = (int)exps[i];
        int      pmod = (int)(p % 7);

        /* Compute p^k (mod 7) for k = 0..e. */
        int powmod[65];           /* 65 is safe for any exponent of a 64‑bit number */
        powmod[0] = 1 % 7;
        for (int k = 1; k <= e; ++k) {
            powmod[k] = (powmod[k - 1] * pmod) % 7;
        }

        /* Zero the new counts array. */
        for (int r = 0; r < 7; ++r) newc[r] = 0;

        /* Transition: for each existing residue and each power of p. */
        for (int r1 = 0; r1 < 7; ++r1) if (counts[r1]) {
            for (int k = 0; k <= e; ++k) {
                int r2 = (r1 * powmod[k]) % 7;
                newc[r2] += counts[r1];
            }
        }

        /* Copy back to counts for the next prime. */
        for (int r = 0; r < 7; ++r) counts[r] = newc[r];
    }

    /* counts[2] holds the required number of divisors. */
    return (seq_t)counts[2];
}
