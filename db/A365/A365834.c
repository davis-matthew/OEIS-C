/*
 * A365834  a(n) is the first number k with n prime divisors such that, if m
 *          is the next number with n prime divisors, k + m and k - m also
 *          have n prime divisors. In each case the divisors are counted with
 *          multiplicity.
 * https://oeis.org/A365834
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-2) * 5 * 67 for n >= 8; otherwise a(n) is found by
 *            brute‑force search
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   count_prime_factors, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* Use the proven closed form for all n >= 8 */
    if (n >= (idx_t)8) {
        seq_t k = pow2(n - 2);          /* 2^(n-2)   */
        k = k * (seq_t)335;             /* 5 * 67 = 335 */
        return k;
    }

    /* Brute force search for n < 8 */
    for (seq_t k = (seq_t)2; ; ++k) {
        if ((idx_t)count_prime_factors(k) != n) continue;

        /* Find the next number m with n prime divisors */
        seq_t m = k + 1;
        while ((idx_t)count_prime_factors(m) != n) ++m;

        /* Check the sum and difference condition */
        if ((idx_t)count_prime_factors(k + m) == n &&
            (idx_t)count_prime_factors(m - k) == n) {
            return k;
        }
    }
}
