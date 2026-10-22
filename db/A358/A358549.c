/*
 * A358549  Triangle read by rows where row n is reversed partial sums of row
 *          n of the Sierpinski triangle (A047999).
 * https://oeis.org/A358549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = sum_{i=k..r} [ ((i & (r-i)) == 0) ], where r = floor such
 *            that (r+1)(r+2)/2 > n (row index), k = n - r(r+1)/2 (column
 *            index). The sum counts odd binomial coefficients in the tail of
 *            the Sierpinski triangle row
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t etc. */

/* Compute a(n) as defined above. */
seq_t a(idx_t n)
{
    /* Find row r such that cumulative elements up to r exceed n.  */
    idx_t r = 0;
    while (1) {
        /* cumulative count up to and including row r */
        unsigned __int128 cum = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (cum > n) break;
        ++r;
    }

    /* Column index within the row.  */
    idx_t prev_cum = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - prev_cum;  /* 0 ≤ k ≤ r */

    /* Count odd binomial coefficients C(r,i) mod 2 for i = k..r.
       A binomial coefficient is odd iff (i & (r-i)) == 0. */
    seq_t sum = 0;
    for (idx_t i = k; i <= r; ++i) {
        if ( (i & (r - i)) == 0 )
            ++sum;
    }
    return sum;
}
