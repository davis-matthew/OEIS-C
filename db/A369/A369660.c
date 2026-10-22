/*
 * A369660  a(n) = 1 if n' is of the form 4k+2, and n' has an even number of
 *          prime factors, otherwise a(n) = 0, where n' stands for the
 *          arithmetic derivative of n, A003415.
 * https://oeis.org/A369660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the arithmetic derivative n' satisfies * n' ≡ 2 (mod
 *            4) and n' has an even number of prime factors * (counted with
 *            multiplicity); otherwise a(n) = 0. * * The function computes n'
 *            by factoring n: * n' = n * Σ(e/p) over all prime powers p^e | n.
 *            * It then tests the two congruence condit...
 * Range:     u8 n <= 4896, u16 n <= 9381, u32 n <= 9381, u64 n <= 9381,
 *            u128 n <= 9381, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>          /* for uint64_t */
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* For n ≤ 1 the arithmetic derivative is 0 → a(n) = 0. */
    if (n <= 1)
        return (seq_t)0;

    /* Compute n' (arithmetic derivative). */
    seq_t deriv = (seq_t)0;
    uint64_t temp = (uint64_t)n;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* deriv += e * n / p */
            deriv += (seq_t)((seq_t)n / (seq_t)p) * (seq_t)e;
        }
    }
    if (temp > 1) {                     /* remaining prime factor */
        deriv += (seq_t)((seq_t)n / (seq_t)temp);  /* e = 1 */
    }

    /* Condition 1: n' ≡ 2 (mod 4). */
    bool cond1 = ((deriv & (seq_t)3) == (seq_t)2);

    /* Condition 2: n' has an even number of prime factors. */
    bool cond2 = ((count_prime_factors(deriv) & (seq_t)1) == (seq_t)0);

    return (cond1 && cond2) ? (seq_t)1 : (seq_t)0;
}
