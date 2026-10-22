/*
 * A342769  Pairwise listing of the partitions of 2k into two parts, (s,t),
 *          with 0 < s <= t ordered by increasing values of s and where k =
 *          1,2,... .
 * https://oeis.org/A342769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Let k = smallest integer with k*(k+1) ≥ n. Let start =
 *            (k‑1)*k + 1, offset = n – start. Let i = offset/2 + 1. If offset
 *            is even, a(n)=i; otherwise a(n)=2*k – i
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined, but harmless */
        return (seq_t)0;

    /* Find the block index k such that (k-1)*k < n ≤ k*(k+1). */
    idx_t low = 1, high = 1;
    /* Increase high until it satisfies the inequality.  */
    while ((__uint128_t)high * (high + 1) < n)
        high <<= 1;

    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((__uint128_t)mid * (mid + 1) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;                     /* correct block number */

    /* Offset inside the block. */
    idx_t start  = (k - 1) * k + 1;    /* 1‑based index of first term of block k */
    idx_t offset = n - start;          /* 0‑based index inside block 0 … 2k‑1 */

    idx_t i = offset / 2 + 1;          /* the s‑value of the pair */
    if ((offset & 1) == 0)             /* even offset → first component (s) */
        return (seq_t)i;
    else                               /* odd offset → second component (t) */
        return (seq_t)(2 * k - i);
}
