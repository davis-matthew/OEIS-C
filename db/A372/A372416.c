/*
 * A372416  Coefficient of x^n in the expansion of ( (1-x+x^3) / (1-x)
 *          )^(2*n).
 * https://oeis.org/A372416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n,k) *
 *            binomial(n-2*k-1,n-3*k)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) is defined to be 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t maxk = n / 3;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* the second binomial is zero unless n-2*k-1 >= n-3*k */
        idx_t a = n - 2*k - 1;
        idx_t b = n - 3*k;
        if (a < b)
            continue;

        seq_t c1 = binomial((idx_t)(2*n), k);
        seq_t c2 = binomial(a, b);
        sum += c1 * c2;   /* multiplication is defined for seq_t */
    }

    return sum;
}
