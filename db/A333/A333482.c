/*
 * A333482  a(n) = [x^(2*n)] S(x)^n, where S(x) = (1 - x - sqrt(1 - 6*x +
 *          x^2))/(2*x) is the o.g.f. of the large Schröder numbers A006318.
 * https://oeis.org/A333482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0, a(n) = (1/3) * Sum_{k = 0..2n} C(3n,k) *
 *            C(5n-k-1, 3n-1) for n >= 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* uses the binomial(x,y) helper from ../utils/binomial.h */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* base case */
    if (n == 0)
        return 1;

    /* constants that will be used in the loop */
    idx_t three_n = 3 * n;           /* 3n */
    idx_t r       = three_n - 1;     /* 3n-1 */

    /* initial binomial values */
    seq_t bin1 = 1;                                   /* C(3n,0)   */
    seq_t bin2 = binomial(5 * n - 1, r);              /* C(5n-1,3n-1) */

    seq_t sum = 0;
    for (idx_t k = 0; k <= 2 * n; ++k)
    {
        /* add term of the sum */
        sum += bin1 * bin2;

        if (k == 2 * n)      /* last iteration, no need to update */
            break;

        /* update bin1 : C(3n,k+1) = C(3n,k) * (3n-k)/(k+1) */
        bin1 = bin1 * (seq_t)(three_n - k) / (seq_t)(k + 1);

        /* update bin2 : C(5n-(k+1)-1,3n-1) from C(5n-k-1,3n-1) */
        idx_t m_k = 5 * n - k - 1;                    /* current m = 5n-k-1 */
        bin2 = bin2 * (seq_t)(m_k - r) / (seq_t)m_k;
    }

    /* divide by 3 as required by the closed formula */
    return sum / (seq_t)3;
}
