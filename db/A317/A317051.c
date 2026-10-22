/*
 * A317051  Triangle read by rows: T(0,0) = 1; T(n,k) = T(n-1,k) + 9 *
 *          T(n-2,k-1) for k = 0..floor(n/2); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A317051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   a(n) enumerates the triangle defined by * T(n,k) = 9^k *
 *            binomial(n-k, k), for 0 ≤ k ≤ floor(n/2), * row by row
 *            (row-major). a(0) = T(0,0) = 1
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Determine the row (row) and column (k) corresponding to
     * the n‑th element in the row‑major ordering. */
    idx_t row   = 0;
    idx_t count = 0;                     /* entries before the current row */
    while (1) {
        idx_t entries = (row / 2) + 1;    /* floor(row/2)+1 elements in this row */
        if (count + entries > n)
            break;
        count += entries;
        ++row;
    }

    idx_t k = n - count;                  /* column within the row */

    /* Compute 9^k * binomial(row-k, k). */
    seq_t pow9   = pow_uint(9, k);
    seq_t binom  = binomial(row - k, k);

    return pow9 * binom;
}
