/*
 * A370232  Triangle read by rows. T(n, k) = binomial(n + k, 2*k)^2.
 * https://oeis.org/A370232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r + k, 2*k)^2 where r = the row index such that
 *            r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n - r*(r+1)/2 (column
 *            within that row)
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                  /* for uint64_t (used in idx_t) */
#include <stdbool.h>                 /* required by some helpers */
#include "../utils/binomial.h"       /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Find row r: smallest r such that r*(r+1)/2 > n
       (i.e., n lies in row r-1).  Use a simple incremental search
       convenient for n up to about 10^5. */
    idx_t r   = 0;
    idx_t acc = 0;        /* number of elements before current row */

    while (acc + (r + 1) <= n) {
        acc += r + 1;
        ++r;
    }

    idx_t k   = n - acc;                /* column in row r */
    idx_t m   = r + k;                  /* n + k  */
    idx_t p   = 2 * k;                  /* 2*k   */

    seq_t bin = binomial(m, p);
    return bin * bin;
}
