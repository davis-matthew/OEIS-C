/*
 * A387013  E.g.f. A(x) satisfies A(x) = exp(x * (1+x^2)^2 * A(x)).
 * https://oeis.org/A387013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (n-2k+1)^(n-2k-1) *
 *            binomial(2*(n-2k),k)/(n-2k)!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t sum = (seq_t)0;
    idx_t m = n;          /* current m = n-2*k  */
    seq_t ratio = (seq_t)1;   /* n! / m!  */

    for (idx_t k = 0; k <= n/2; ++k) {
        seq_t pow_val;
        if (m == 0)
            pow_val = (seq_t)1;                    /* (1)^{-1} is treated as 1 */
        else
            pow_val = seq_pow((seq_t)(m + 1), (idx_t)(m - 1));

        seq_t bin = binomial(2 * m, k);

        seq_t term = seq_mul(seq_mul(ratio, pow_val), bin);
        sum = sum + term;                /* addition of seq_t values */

        if (m >= 2) {                    /* update ratio for next m = m-2 */
            ratio = seq_mul(ratio, (seq_t)(m - 1));
            ratio = seq_mul(ratio, (seq_t)m);
        }
        m -= 2;
    }
    return sum;
}
