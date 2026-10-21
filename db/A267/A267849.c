/*
 * A267849  Triangular array: T(n,k) is the 2-row creation rook number to
 *          place k rooks on a 3 x n board.
 * https://oeis.org/A267849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,c) where r is the first integer with (r+1)(r+2)/2 > n
 *            and c = n - r(r+1)/2. T(r,c) = binomial(c+2,2) *
 *            ∏_{i=0}^{c-1}(r-i)
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 86, u64 n <= 207,
 *            u128 n <= 590, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Binary search for the row r: smallest r with S(r) > n, where
       S(r) = (r+1)(r+2)/2 is the total number of elements up to row r. */
    idx_t low = 0;
    idx_t high = n;                     /* S(high) > n for all n >= 0 */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t sum = ((seq_t)(mid + 1) * (mid + 2)) / 2;   /* S(mid) */
        if (sum > (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                       /* row index */

    /* Compute previous cumulative count S(r-1) and the column c. */
    seq_t prev = (r == 0) ? 0u : ((seq_t)(r) * (r + 1)) / 2; /* S(r-1) */
    seq_t diff = (seq_t)n - prev;
    idx_t c = (idx_t)diff;               /* 0 <= c <= r */

    /* binomial(c+2,2) = (c+2)(c+1)/2 */
    seq_t bin = ((seq_t)(c + 2) * (c + 1)) / 2;

    /* falling factorial r!/(r-c)! = ∏_{i=0}^{c-1}(r-i) */
    seq_t ff = 1;
    for (idx_t i = 0; i < c; ++i)
        ff *= (seq_t)(r - i);

    return bin * ff;
}
