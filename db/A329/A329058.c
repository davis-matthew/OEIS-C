/*
 * A329058  2-parking triangle T(r, i, 2) read by rows: T(r, i, k) = (r +
 *          1)^(i-1)*binomial(k*(r + 1) + r - i - 1, r - i) with k = 2 and 0
 *          <= i <= r.
 * https://oeis.org/A329058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Let r be the largest integer with r*(r+1)/2 ≤ n. Let i =
 *            n − r*(r+1)/2 (0 ≤ i ≤ r). If i == 0: a(n) = binomial(3*r+1, r)
 *            / (r+1). Else: a(n) = (r+1)^(i-1) * binomial(3*r+1-i, r-i).
 *            (Here binomial(n,k) returns C(n,k) as seq_t.)
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 62, u64 n <= 164, u128 n <= 427,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"
#include "../utils/binomial.h"

/* Find the row r that contains index n.
   r satisfies r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.  */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* Grow high until the triangular number exceeds n. */
    while (((__int128)high * (high + 1) / 2) <= n) {
        low = high;
        high <<= 1;
    }
    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (((__int128)mid * (mid + 1) / 2) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine row r and column i. */
    idx_t r = find_row(n);                 /* row number */
    idx_t start = r * (r + 1) / 2;          /* first index in this row */
    idx_t i = n - start;                   /* column index (0‑based) */

    if (i == 0) {
        /* First entry of the row:  T(r,0,2) = binomial(3r+1, r)/(r+1). */
        seq_t bin = binomial(3 * r + 1, r);
        return bin / (r + 1);
    } else {
        /* General entry:  (r+1)^(i-1) * binomial(3r+1-i, r-i). */
        seq_t powfac = pow_uint((uint64_t)(r + 1), i - 1);
        seq_t bin = binomial(3 * r + 1 - i, r - i);
        return powfac * bin;
    }
}
