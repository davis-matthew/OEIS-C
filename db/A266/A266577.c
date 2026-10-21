/*
 * A266577  Square array read by descending antidiagonals: T(n,k) = ((2^(n+1)
 *          + 1)^(k-1) + 1)/2.
 * https://oeis.org/A266577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((2^(row+1)+1)^(col-1)+1)/2 read by descending
 *            antidiagonals
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 57, u64 n <= 124,
 *            u128 n <= 239, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* row = position in diagonal + 1, col = diagonal index - position */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset is 1, so n=0 is unused */

    /* Find the diagonal index t (number of elements in the diagonal).  */
    idx_t t = 1;
    while (t * (t + 1) / 2 < n) {
        ++t;                           /* O(sqrt(n)) for n up to 10000 */
    }

    /* Elements preceding this diagonal */
    idx_t prev = (t - 1) * t / 2;

    /* Position within the diagonal (0‑based) */
    idx_t pos = n - prev - 1;          /* 0 ≤ pos < t */

    /* Map to (row, col) in the triangular array */
    idx_t row = pos + 1;               /* 1 … t  */
    idx_t col = t - pos;               /* 1 … t  */

    /* Compute ((2^(row+1)+1)^(col-1)+1)/2 */
    seq_t power_of_two = ((seq_t)1 << (row + 1));   /* 2^(row+1) */
    seq_t base = power_of_two + (seq_t)1;           /* 2^(row+1)+1 */
    seq_t power = big_pow(base, (seq_t)(col - 1));
    seq_t result = (power + (seq_t)1) / (seq_t)2;

    return result;
}
