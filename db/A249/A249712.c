/*
 * A249712  Number of length 6+3 0..n arrays with every four consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A249712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 52*binom(n-1,0) + 607*binom(n-1,1) + 2174*binom(n-1,2) +
 *            3530*binom(n-1,3) + 2856*binom(n-1,4) + 1088*binom(n-1,5) +
 *            144*binom(n-1,6) (binom(m,k) = 0 for m < k) This is equivalent
 *            to the provided degree‑6 polynomial and the 7‑term recurrence.
 *            The implementation uses only integer arithmetic...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 49, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but keeps function defined */
    idx_t t = n - 1;                /* shift to use binom(t,k) */

    static const seq_t coeff[7] = {
        52,          /* k = 0 */
        607,         /* k = 1 */
        2174,        /* k = 2 */
        3530,        /* k = 3 */
        2856,        /* k = 4 */
        1088,        /* k = 5 */
        144          /* k = 6 */
    };

    seq_t result = 0;

    for (idx_t k = 0; k <= 6; ++k) {
        seq_t binom = 1;
        /* compute binom(t,k) */
        for (idx_t i = 1; i <= k; ++i) {
            if (t < i - 1) {        /* t < k ⇒ binom is 0 */
                binom = 0;
                break;
            }
            binom = binom * (seq_t)(t - i + 1);
            binom = binom / (seq_t)i;   /* exact division */
        }
        result += coeff[k] * binom;
    }
    return result;
}
