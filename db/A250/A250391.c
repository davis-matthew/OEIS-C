/*
 * A250391  Number of length 5+3 0..n arrays with no four consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A250391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..6} s_k * binomial(n + 7 - k, 8) where s = {6,
 *            460, 5070, 15078, 14644, 4670, 392} (These constants come from 2
 *            * {3, 230, 2535, 7539, 7322, 2335, 196})
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b); */

static const seq_t coeffs[7] = {
    (seq_t)6,
    (seq_t)460,
    (seq_t)5070,
    (seq_t)15078,
    (seq_t)14644,
    (seq_t)4670,
    (seq_t)392
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* behaviour below offset not required */
    seq_t sum = 0;
    idx_t base = n + 7;              /* n + 7 = (n-1) + 8 */

    for (idx_t k = 0; k < 7; ++k) {
        idx_t t = base - k;          /* t = n + 7 - k = (n-1-k)+8 */
        if (t >= 8) {
            seq_t term = binom(t, 8);
            /* multiply by the small integer coefficient */
            sum += coeffs[k] * term;
        }
    }
    return sum;
}
