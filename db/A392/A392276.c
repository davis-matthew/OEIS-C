/*
 * A392276  The maximum exponent in the prime factorization of the square root
 *          of the largest square dividing n.
 * https://oeis.org/A392276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(A051903(n)/2): the maximum exponent in the prime
 *            factorization of n, divided by two
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;        /* a(1) = 0 */

    uint64_t tmp = (uint64_t)n;        /* n fits in uint64_t */
    seq_t max_exp = 0;

    /* trial‑division using 64‑bit arithmetic, no overflow on p*p */
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            seq_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            if (exp > max_exp) max_exp = exp;
        }
    }

    /* any remaining factor > 1 is prime with exponent 1 */
    if (tmp > 1) {
        if ((seq_t)1 > max_exp) max_exp = 1;
    }

    return max_exp / 2;   /* floor division by 2 */
}
