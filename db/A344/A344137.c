/*
 * A344137  Sum of the squarefree divisors of n whose square does not divide
 *          n.
 * https://oeis.org/A344137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{p|n} (1+p) – ∏_{p^2|n} (1+p) (sum of squarefree
 *            divisors of n whose square does not divide n)
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* for n = 1 the answer is 0 */
    if (n < 2)
        return 0;

    idx_t tmp = n;

    seq_t prod_all = 1;   /* product of (1 + p) for all prime factors */
    seq_t prod_sq  = 1;   /* product of (1 + p) for prime factors with exponent ≥ 2 */

    /* factor 2 separately */
    if ((tmp & 1) == 0) {
        int exp = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++exp;
        }
        prod_all *= (seq_t)2 + 1;
        if (exp >= 2)
            prod_sq *= (seq_t)2 + 1;
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            prod_all *= (seq_t)p + 1;
            if (exp >= 2)
                prod_sq *= (seq_t)p + 1;
        }
    }

    /* if a prime factor remains */
    if (tmp > 1) {
        prod_all *= (seq_t)tmp + 1;
        /* exponent is 1, so it does not contribute to prod_sq */
    }

    return prod_all - prod_sq;
}
