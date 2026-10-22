/*
 * A365403  The sum of the unitary divisors of the largest square dividing n.
 * https://oeis.org/A365403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (1 + p^(2*⌊e/2⌋)) (a(p) = 1 for e = 1)
 * Range:     u8 n <= 224, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Compute the sum of the unitary divisors of the largest square dividing n. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;   /* a(1) = 1 */

    idx_t m = n;           /* work on a copy of n */
    seq_t result = (seq_t)1;

    /* trial division up to √m */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (exp > 1) {
                idx_t even_exp = (exp / 2) * 2;   /* largest even ≤ exp */
                seq_t pow = (seq_t)1;
                for (idx_t i = 0; i < even_exp; ++i)
                    pow *= (seq_t)p;
                result *= (pow + (seq_t)1);
            }
            /* exp == 1 contributes factor 1: nothing to change */
        }
    }

    /* If a prime factor remains > sqrt(n), its exponent is 1
       and contributes factor 1, so no change is needed. */

    return result;
}
