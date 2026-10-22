/*
 * A381968  a(a(n)) = A381662(n).
 * https://oeis.org/A381968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth element of the triangular array defined in OEIS
 *            A381968. The array is read row by row, each row n having length
 *            4n-3. The row consists of a permutation of the consecutive block
 *            [s(n), s(n)+4n-4] with s(n) = 2n² – 5n + 4 (block start) For
 *            position k in the row (1‑based) the value i...
 * Range:     u8 n <= 231, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the row number r such that 2r² – r  >=  n  and  2(r−1)² – (r−1) < n  */
static idx_t
row_for_n(idx_t n)
{
    idx_t low = 1, high = 1;
    /* increase high until the inequality is satisfied */
    while (((unsigned __int128)2 * high * high - high) < n)
        high <<= 1;
    /* binary search for the minimal r satisfying 2r² – r >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = ((unsigned __int128)2 * mid * mid) - mid;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset is 1; n=0 is not used */

    /* row number of the nth element */
    idx_t r = row_for_n(n);

    /* position k within the row (1‑based) */
    idx_t prev_total = (unsigned __int128)2 * (r - 1) * (r - 1) - (r - 1);
    idx_t k = n - prev_total;            /* 1 <= k <= 4r-3 */

    /* constants for this row */
    idx_t m = 2 * r - 1;
    __int128 base128 = (__int128)2 * r * r - (__int128)5 * r + 3;
    seq_t base = (seq_t)base128;
    seq_t result;

    if (k < m) {
        if (k & 1) {                     /* k odd  */
            result = base + ((seq_t)(2 * m - 1) - (seq_t)k);
        } else {                         /* k even */
            result = base + (seq_t)k;
        }
    } else {                             /* k >= m */
        if (k & 1) {                     /* k odd  */
            result = base + (seq_t)k;
        } else {                         /* k even */
            result = base + ((seq_t)k - (seq_t)m);
        }
    }

    return result;
}
