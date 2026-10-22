/*
 * A356811  a(n) = Sum_{k=0..n} (k*n+1)^(n-k) * binomial(n,k).
 * https://oeis.org/A356811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k*n+1)^(n-k) * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* compute base = k*n + 1 */
        seq_t base = (seq_t)k * n + 1;
        /* exponent = n - k  */
        idx_t exp = n - k;

        /* fast exponentiation: base^exp */
        seq_t pow_val = 1;
        seq_t cur = base;
        while (exp) {
            if (exp & 1u)
                pow_val *= cur;
            exp >>= 1;
            if (exp)
                cur *= cur;
        }

        /* term = pow_val * binomial(n,k) */
        seq_t term = pow_val * binomial(n, k);
        sum += term;
    }
    return sum;
}
