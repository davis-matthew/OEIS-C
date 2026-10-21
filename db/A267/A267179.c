/*
 * A267179  Irregular triangle read by rows: successive bottom and right-hand
 *          borders of the infinite square array in A072030 (number of
 *          subtraction steps needed to compute GCD) read mod 2.
 * https://oeis.org/A267179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = parity of the number of subtraction steps in the
 *            Euclidean subtraction algorithm (counted as “steps‑1”) for the
 *            pair (i,j) determined from n by unfolding the infinite array
 *            A072030 along successive bottom and right borders. For n > 0 let
 *            m = ⌊√(n‑1)⌋ + 1, let p = n – (m‑1)² – 1 (0‑based of...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   parity_substeps
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/parity_substeps.h"

/* Integer square root: floor(sqrt(n)) for n up to 2^64-1. */
static idx_t int_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = 1ULL << 32; /* since (2^32)^2 = 2^64 */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)   /* just in case */
        return 0;

    /* Determine the size m of the square that contains n. */
    idx_t m = int_sqrt_u64(n - 1) + 1;
    idx_t prev = (m - 1) * (m - 1);
    idx_t p = n - prev - 1; /* 0‑based offset in the current border */

    idx_t i, j;
    if (p < m) {
        i = m;
        j = p + 1;
    } else {
        i = p - m + 1;
        j = m;
    }

    return parity_substeps(i, j);
}
