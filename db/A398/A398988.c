/*
 * A398988  The binomial matrix of the number of endofunctions n^n, square
 *          array read by ascending antidiagonals.
 * https://oeis.org/A398988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(i,j) where (i,j) are the row/column indices at position
 *            n in the array read by ascending antidiagonals. For those
 *            indices A(i,j) = Σ_{m=0}^{|i-j|} C(|i-j|,m) · s^{|i-j|-m} ·
 *            (min(i,j)+m)^{min(i,j)+m}, where s = 1 if j ≤ i else –1
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/binom.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the diagonal sum index d_sum = i + j by cumulative counts. */
    idx_t d_sum = 0;          /* current diagonal sum index */
    idx_t cum   = 0;          /* cumulative number of elements before this diagonal */

    while (cum + d_sum + 1 <= n) {
        cum += d_sum + 1;     /* size of current diagonal is d_sum + 1 */
        d_sum++;
    }

    /* Position inside this diagonal. */
    idx_t offset = n - cum;          /* 0‑based index within diagonal */
    idx_t i = d_sum - offset;        /* row index */
    idx_t j = offset;                /* column index */

    /* Parameters for the binomial sum. */
    idx_t diff = (i > j) ? (i - j) : (j - i);   /* |i-j| */
    idx_t p    = (i < j) ? i : j;               /* min(i,j) */
    int sgn    = (j <= i) ? 1 : -1;             /* s = 1 if j ≤ i, else -1 */

    /* Accumulate contributions with correct sign. */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t m = 0; m <= diff; ++m) {
        seq_t bin = binom(diff, m);

        /* Compute (p+m)^(p+m); treat 0^0 as 1. */
        seq_t base  = (seq_t)(p + m);
        seq_t exp   = base;
        seq_t power = (base == (seq_t)0) ? 1 : big_pow(base, exp);

        seq_t term = bin * power;

        if (sgn == 1) {
            /* s = 1, all terms are positive. */
            sum_pos += term;
        } else {
            /* s = -1, sign depends on parity of (diff - m). */
            if (((diff - m) & 1U) == 0U) {
                sum_pos += term;   /* positive contribution */
            } else {
                sum_neg += term;   /* negative contribution */
            }
        }
    }

    return sum_pos - sum_neg;
}
