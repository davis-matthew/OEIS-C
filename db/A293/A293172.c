/*
 * A293172  Triangle read by rows: T(n,k) = number of colored weighted Motzkin
 *          paths ending at (n,k).
 * https://oeis.org/A293172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A293172
   Formula: a(n) = T(row, col)  where
       row = floor((sqrt(8*n+1)-1)/2)
       col = n - row*(row+1)/2
       r   = row - col           (0 ≤ r ≤ row)
   T(row, col) = 2^{r} * binomial(2*row+1, r)
   binomial(2*row+1, r) is computed multiplicatively.
*/

#include <stdbool.h>
#include <stdint.h>

static inline idx_t
row_of_n(idx_t n)
{
    /* find the largest r such that r*(r+1)/2 <= n */
    idx_t low = 0;
    idx_t high = (idx_t)(2 * n);          /* high is surely enough */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    idx_t row = row_of_n(n);
    idx_t before = row * (row + 1) / 2;
    idx_t col   = n - before;
    idx_t r     = row - col;              /* exponent and binomial upper part */

    /* compute binomial(2*row+1, r) multiplicatively */
    seq_t binom = 1;
    for (idx_t i = 1; i <= r; ++i) {
        /* numerator: 2*row+2-i  (since 2*row+1 - r + i = 2*row+2-i) */
        binom = (binom * ((seq_t)(2 * row + 2 - i))) / ((seq_t)i);
    }

    /* multiply by 2^r */
    for (idx_t i = 0; i < r; ++i)
        binom *= 2;

    return binom;
}
