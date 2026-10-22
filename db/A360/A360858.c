/*
 * A360858  Triangle read by rows. T(n, k) = binomial(n + 1, ceil(k/2)) *
 *          binomial(n, floor(k/2)).
 * https://oeis.org/A360858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) with r = largest integer such that r*(r+1)/2 <= n,
 *            k = n - r*(r+1)/2, T(r,k) = binomial(r+1, ceil(k/2)) *
 *            binomial(r, floor(k/2))
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t row = 0;
    /* find r so that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    while ( (( (__int128)(row + 1) * (row + 2)) / 2 ) <= n )
        row++;

    idx_t start = row * (row + 1) / 2;   /* first index of this row */
    idx_t k     = n - start;              /* column within the row */

    idx_t half1 = (k + 1) / 2;            /* ceil(k/2)   */
    idx_t half2 = k / 2;                  /* floor(k/2)  */

    seq_t part1 = binomial(row + 1, half1);
    seq_t part2 = binomial(row,     half2);

    return part1 * part2;
}
