/*
 * A322914  a(0)=0; for n>0, a(n) is the number of rooted 4-regular maps on
 *          the torus having n vertices.
 * https://oeis.org/A322914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^(n-1) * (2^(2n) - C(2n,n))) / 2 for n > 0 (same as
 *            a(n) = 6^(n-1)*(2^n - (2n-1)!!/n!))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /**  Compute binomial C(2n,n) using the multiplicative
         formula:  C(2n,n) = prod_{k=1}^{n} (n+k)/k          */
    seq_t binom = 1;
    for (idx_t k = 1; k <= n; ++k) {
        binom = seq_mul(binom, (seq_t)(n + k));
        binom /= (seq_t)k;           /* exact division */
    }

    /**  2^(2n) = 4^n                                   */
    seq_t pow4 = seq_pow((seq_t)4, n);

    /**  diff = 2^(2n) - C(2n,n)                         */
    seq_t diff = pow4 - binom;

    /**  3^(n-1)                                         */
    seq_t pow3 = seq_pow((seq_t)3, n - 1);

    /**  a(n) = (3^(n-1) * diff) / 2                     */
    seq_t tmp = seq_mul(diff, pow3);
    seq_t result = tmp / (seq_t)2;

    return result;
}
