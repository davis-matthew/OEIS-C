/*
 * A277484  E.g.f.: -arcsinh(x)*LambertW(-x).
 * https://oeis.org/A277484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * sum_{r=0}^{⌊(n-2)/2⌋} (-1)^r * binom(2r,r) *
 *            (n-2r-1)^{n-2r-2} / ((n-2r-1)! * 4^r * (2r+1)) Offset: 0
 *            (a(0)=0, a(1)=0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* trivially 0 for n = 0,1 */
    if (n < 2)
        return 0;

    idx_t maxr = (n - 2) / 2;
    seq_t sum = 0;

    /* ratio = n! / (n-1)! = n  for r = 0 */
    seq_t ratio = (seq_t)n;

    /* binom = binom(0,0) = 1 for r = 0 */
    seq_t binom = 1;

    for (idx_t r = 0; r <= maxr; ++r) {
        idx_t j = n - 2 * r - 1;            /* j >= 1 */
        seq_t powj = pow_uint(j, j - 1);     /* j^(j-1) */

        /* numerator = ratio * binom * powj */
        seq_t num = ratio;
        num *= binom;
        num *= powj;

        /* denominator = 4^r * (2r+1) */
        seq_t denom = pow_uint(4, r);
        denom *= (2 * r + 1);

        seq_t term = num / denom;           /* exact division */

        if (r & 1)     /* sign alternates */
            sum -= term;
        else
            sum += term;

        /* prepare for next r, if any */
        if (r == maxr)
            break;

        /* j is the current value of j for this r */
        seq_t j_seq   = (seq_t)j;
        seq_t j_minus = j_seq - 1;
        ratio *= j_seq;
        ratio *= j_minus;

        /* update binom for next r:
           binom_{r+1} = binom_r * (2r+2)(2r+1) / ((r+1)(r+1))   */
        seq_t num_b = binom;
        num_b *= (2 * r + 2);
        num_b *= (2 * r + 1);

        seq_t denom_b = (r + 1);
        denom_b *= (r + 1);

        binom = num_b / denom_b;
    }

    return sum;
}
