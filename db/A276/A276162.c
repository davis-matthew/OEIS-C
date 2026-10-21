/*
 * A276162  Square array read by antidiagonals: T(n,k) = Product_{i = 1..k}
 *          gcd(n, i).
 * https://oeis.org/A276162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ∏_{i=1}^k gcd(r,i) where (r,k) is the cell of the
 *            infinite array T(n,k) located on the antidiagonal that contains
 *            index n. The antidiagonals are read from the bottom‑left element
 *            upward: the first element of each diagonal is (r = s-1, k = 1)
 * Range:     u8 n <= 99, u16 n <= 270, u32 n <= 729, u64 n <= 2004,
 *            u128 n <= 6429, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the antidiagonal number s such that
       (s-2)*(s-1)/2 < n ≤ (s-1)*s/2. */
    idx_t high = 2;
    while ((unsigned __int128)high * (high - 1) / 2 < n)
        high <<= 1;

    idx_t low = 2;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * (mid - 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;                     /* antidiagonal number */

    /* Offset within this diagonal (1 … s-1). */
    idx_t prev_sum = (s - 2) * (s - 1) / 2;
    idx_t offset   = n - prev_sum;

    /* Row and column on this diagonal (bottom‑left to top‑right). */
    idx_t row = s - offset;            /* r = s - offset      */
    idx_t col = offset;                /* k = offset          */

    /* Compute T(row,col) = ∏_{i=1}^col gcd(row,i) */
    seq_t prod = 1;
    for (idx_t i = 1; i <= col; ++i)
        prod *= gcd((seq_t)row, (seq_t)i);

    return prod;
}
