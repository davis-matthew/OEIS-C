/*
 * A386490  Array read by ascending antidiagonals: A(n,m) =
 *          numerator(2n*m/(m^2 + 1)), where m > 0.
 * https://oeis.org/A386490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, tabl
 * Formula:   a(n) = numerator(2 * i * j / (j^2 + 1)) read by ascending
 *            antidiagonals (OEIS A386490)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d (sum of indices) that contains position n.
       Diagonal d has length d-1 and the cumulative count up to this
       diagonal is d*(d-1)/2.  We increment d until this count reaches n.  */
    idx_t d = 2;            /* first diagonal n+m = 2 (contains (1,1)) */
    idx_t count = 1;        /* elements processed up to current diagonal */

    while (count < n) {
        d++;
        count += d - 1;     /* add this diagonal's length */
    }

    /* Count of elements before this diagonal */
    idx_t prev_count = count - (d - 1);

    /* Position within the diagonal (0-based) */
    idx_t pos = n - prev_count - 1;

    /* Compute the corresponding n (row) and m (column) values.
       Within each diagonal we traverse with n decreasing, m increasing. */
    idx_t n_val = d - 1 - pos;  /* row index (>=1) */
    idx_t m_val = 1 + pos;      /* column index (>=1) */

    /* Compute numerator and denominator of the fraction 2*n_val*m_val/(m_val^2+1). */
    seq_t num = (seq_t)2 * (seq_t)n_val * (seq_t)m_val;
    seq_t den = (seq_t)m_val * (seq_t)m_val + 1;

    /* Reduce the fraction to lowest terms and return the numerator. */
    seq_t g = gcd(num, den);
    return num / g;
}
