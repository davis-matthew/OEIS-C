/*
 * A279290  Sum of cubes of nonprime divisors of n.
 * https://oeis.org/A279290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001158(n) - A005064(n)
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term; for n ≤ 1 we return 1 directly */
    if (n <= 1)
        return (seq_t)1;

    /* Buffers for prime factors and exponents (maximum 64 distinct primes
       sufficient for any 64‑bit number). */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    /* Factor n into primes. */
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t sigma3 = 1;          /* product of sigma3(p^e) over all primes */
    seq_t sum_prime_cubes = 0; /* sum of p^3 over all distinct primes */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)pr[i];
        seq_t p3  = p * p * p;           /* p^3 */

        /* Compute 1 + p^3 + p^6 + … + p^(3*e) */
        seq_t term = 1;                  /* sum for this prime power */
        seq_t pow  = 1;                  /* current power of p^3 */
        for (int j = 0; j < (int)ex[i]; ++j) {
            pow  *= p3;                  /* p^(3*(j+1)) */
            term += pow;
        }

        sigma3          *= term;
        sum_prime_cubes += p3;
    }

    return sigma3 - sum_prime_cubes;
}
