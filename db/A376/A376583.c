/*
 * A376583  Parity of A002260.
 * https://oeis.org/A376583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 if the column index of n in the alternating 1/0
 *            triangle (row i has entries 1,0,1,0,... starting with 1) is odd,
 *            otherwise 0
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The triangle is           1
                               1 0
                               1 0 1
                               1 0 1 0
                               1 0 1 0 1
                               ...                               */

#include <stdint.h>

/* Return the row index i (1‑based) such that
   i*(i+1)/2 >= n and (i-1)*i/2 < n. */
static idx_t row_from_n(idx_t n)
{
    idx_t low = 1, high = 1;
    /* exponential search for an upper bound */
    while (((unsigned __int128)high * (high + 1) / 2) < n)
        high <<= 1;
    /* binary search */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (((unsigned __int128)mid * (mid + 1) / 2) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine the row i that contains n. */
    idx_t i = row_from_n(n);

    /* Number of elements before row i (triangular number of i-1). */
    idx_t prev = ((unsigned __int128)(i - 1) * i) / 2;

    /* Column index within the row (1‑based). */
    idx_t col = n - prev;

    /* Return 1 if col is odd, else 0. */
    return (seq_t)(col & 1);
}
