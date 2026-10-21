/*
 * A259433  Irregular table read by rows: suppose n has d decimal digits; then
 *          T(n,j) for 1 <= j <= d is the number obtained by discarding the
 *          d-j least significant digits of n.
 * https://oeis.org/A259433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) = floor(k / 10^(digits(k)-j)) where k is the unique integer
 *            with S(k-1) < n ≤ S(k), S(k)=∑_{i=1}^k digits(i), and j = n -
 *            S(k-1). Here digits(i) is the decimal number of digits of i.
 *            This implements the irregular table read by rows description of
 *            A259433
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

/* Count decimal digits of an idx_t number (assumes x≥1). */
static inline int digit_count_idx(idx_t x)
{
    int c = 0;
    while (x != 0) {
        c++;
        x /= 10;
    }
    return c;
}

/* Computes a(n) as defined above. */
seq_t a(idx_t n)
{
    idx_t cum = 0;         /* cumulative digit count */
    idx_t k   = 0;         /* current row number */
    idx_t d   = 0;         /* digits in current row */
    idx_t prev_cum = 0;    /* cum before adding current row */
    while (cum < n) {
        k++;
        prev_cum = cum;
        d = digit_count_idx(k);
        cum += d;
    }
    /* Now k is the row containing the nth term, prev_cum = S(k-1) */
    idx_t j = n - prev_cum;                   /* column within the row (1≤j≤d) */
    idx_t exp = d - j;                         /* exponent for truncation */
    seq_t pow = pow10(exp);                    /* 10^(exp) as seq_t */
    seq_t result = ((seq_t)k) / pow;          /* truncated integer */
    return result;
}
