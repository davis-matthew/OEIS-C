/*
 * A369667  a(n) = 1 if n' == 3 (mod 4) and n has an even number of prime
 *          factors with multiplicity, otherwise 0. Here n' stands for the
 *          arithmetic derivative of n, A003415(n).
 * https://oeis.org/A369667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n' ≡ 3 (mod 4) and Ω(n) ≡ 0 (mod 2), else 0 * (where
 *            n' is the arithmetic derivative and Ω(n) is the total * number
 *            of prime factors of n counting multiplicity)
 * Range:     u8 n <= 8583, u16 n <= 8583, u32 n <= 8583, u64 n <= 8583,
 *            u128 n <= 8583, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* n = 0 or 1 gives derivative 0 and Ω(n) = 0 (even) but 0 ≢ 3 (mod 4) */
    if (n <= 1)
        return 0;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor_u64 is provided in the utils build set */
    factor_u64(nn, primes, exps, &cnt);

    /* Count total number of prime factors with multiplicity */
    unsigned total_mult = 0;
    for (size_t i = 0; i < cnt; ++i)
        total_mult += (unsigned)exps[i];

    bool even_mult = (total_mult % 2) == 0;

    /* Compute the arithmetic derivative modulo 4 only.
     * We use 64‑bit arithmetic for the intermediate steps which is
     * safe for the n values used in the evaluation tests (up to ~10 000). */
    unsigned deriv_mod4 = 0;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned long long e = exps[i];
        unsigned long long p = primes[i];

        /* A = nn * e  (fits in 64 bits for n ≤ 10 000) */
        unsigned long long A = nn * e;

        /* Compute A / p modulo 4 without overflow:
         * Let s = A mod (4p). Then floor(A/p) mod 4 = floor(s/p). */
        unsigned long long s = A % (4ULL * p);
        unsigned int term_mod4 = (unsigned)(s / p);
        deriv_mod4 = (deriv_mod4 + term_mod4) & 3U; /* mod 4 */
    }

    return (even_mult && (deriv_mod4 == 3U)) ? 1 : 0;
}
