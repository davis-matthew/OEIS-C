/*
 * A329060  4-parking triangle T(r, i, 4) read by rows: T(r, i, k) = (r +
 *          1)^(i-1)*binomial(k*(r + 1) + r - i - 1, r - i) with k = 4 and 0
 *          <= i <= r.
 * https://oeis.org/A329060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the 4‑parking triangle read by rows.
 *            T(r,i,4) = (r+1)^(i-1) * binomial(4*(r+1)+r-i-1, r-i) for 1 ≤ i
 *            ≤ r T(r,0,4) = binomial(4*(r+1)+r-1, r) / (r+1) (*) a(n) = T(r,
 *            i, 4) where r satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2 and i = n -
 *            r(r+1)/2. (0‑based indexing)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 55, u64 n <= 145, u128 n <= 399,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r and column i such that a(n) = T(r,i,4)          */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        r++;
    idx_t start = r * (r + 1) / 2;   /* first index of this row   */
    idx_t i = n - start;             /* column within the row     */

    if (i == 0) {                    /* special case for (r,0)    */
        seq_t num = binomial(4 * (r + 1) + r - 1, r);
        return num / (seq_t)(r + 1);
    }

    /* (r+1)^(i-1) */
    seq_t power = big_pow((seq_t)(r + 1), (seq_t)(i - 1));

    /* binomial(4*(r+1)+r-i-1, r-i) */
    idx_t n_binom = 4 * (r + 1) + r - i - 1;
    idx_t k_binom = r - i;
    seq_t bin = binomial(n_binom, k_binom);

    return power * bin;
}
