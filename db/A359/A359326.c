/*
 * A359326  Number of divisors of 6*n-4 of form 6*k+5.
 * https://oeis.org/A359326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of 6*n-4 such that d ≡ 5 (mod 6)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset starts at 1 */

    /* Compute m = 6*n - 4, using 128-bit to avoid overflow for small n */
    unsigned __int128 tmp = (unsigned __int128)n * 6 - 4;
    uint64_t m = (uint64_t)tmp;   /* assume m fits in 64‑bit for test data */

    /* Factor m: primes[ ], exps[ ] will hold the distinct primes and their exponents */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    seq_t total = 1;   /* total number of divisors of m' (m with 2,3 removed) */
    seq_t alt   = 1;   /* product over (E_i - O_i) used to enforce odd parity */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* skip factors 2 and 3: they cannot appear in a divisor ≡5 (mod 6) */
        if (p == 2 || p == 3) continue;

        seq_t A = (seq_t)(e + 1);  /* number of choices for exponent of p */
        total *= A;

        if (p % 6 == 5) {
            /* For primes 5 mod 6: (E_i - O_i) is 1 if e even, 0 otherwise */
            seq_t diff = (e % 2 == 0) ? (seq_t)1 : (seq_t)0;
            alt *= diff;
        } else { /* p % 6 == 1 */
            alt *= A;
        }
    }

    /* number of divisors with odd number of odd exponents among 5 mod 6 primes */
    seq_t result = (total - alt) / 2;
    return result;
}
