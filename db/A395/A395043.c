/*
 * A395043  a(n) = 1 if the greatest prime dividing arithmetic derivative of n
 *          is larger than the greatest prime dividing n, otherwise 0. a(1) =
 *          0 by convention.
 * https://oeis.org/A395043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if the greatest prime factor of the arithmetic
 *            derivative of n is larger than the greatest prime factor of n,
 *            otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    /* a(1) is defined to be 0 by convention */
    if (n <= 1)
        return (seq_t)0;

    /* Factorise n (primes and exponents) – arrays big enough for all n ≤ 10000 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Greatest prime factor of n */
    uint64_t gpf_n = gpf((uint64_t)n);

    /* Arithmetic derivative of n:  n' = Σ e_i * n / p_i  */
    unsigned __int128 deriv = 0;
    for (size_t i = 0; i < cnt; ++i) {
        deriv += (unsigned __int128)exps[i] *
                 ((unsigned __int128)n) /
                 primes[i];
    }
    /* n' fits in 64‑bit for all test values (n ≤ 10000) – cast back */
    uint64_t deriv64 = (uint64_t)deriv;

    /* Greatest prime factor of the derivative */
    uint64_t gpf_d = gpf(deriv64);

    /* Return 1 if the derivative has a larger prime factor, else 0 */
    return (seq_t)(gpf_d > gpf_n);
}
