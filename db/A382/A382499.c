/*
 * A382499  Inverse permutation to A381968.
 * https://oeis.org/A382499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A000384(r-1) + P(r,p) where r = smallest integer with
 *            r*(2*r-1) >= n, p = n - A000384(r-1) (1 ≤ p ≤ 4*r-3), m = 2*r-1,
 *            P(r,p) = { p+m if p < m and p odd, p if p < m and p even, p if p
 *            ≥ m and p odd, (2*m-1)-p if p ≥ m and p even }. (A000384(k) =
 *            k*(2*k-1).)
 * Range:     u8 n <= 233, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* out of range, not used */

    /* Find the row r containing position n.  */
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (((unsigned __int128)2 * high) - 1) < n) {
        if (high > (1ULL << 62))  /* avoid overflow if n is very large */
            break;
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 c = (unsigned __int128)mid *
                              (((unsigned __int128)2 * mid) - 1);
        if (c >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;           /* row number (r ≥ 1) */

    /* Compute the start of the block (A000384(r-1)).  */
    idx_t prev = r - 1;
    unsigned __int128 prevC = (unsigned __int128)prev *
                              (((unsigned __int128)2 * prev) - 1);
    seq_t base = (seq_t)prevC;          /* A000384(prev) */

    /* Position within the row.  */
    idx_t p = n - (idx_t)prevC;         /* 1‑based inside the row */

    /* Compute P(r,p).  */
    idx_t m = 2 * r - 1;                /* length of the block is 4*r-3 = 2*m-1 */
    seq_t P;
    if (p < m) {
        if (p & 1)      /* odd */
            P = (seq_t)(p + m);
        else
            P = (seq_t)p;
    } else {
        if (p & 1)      /* odd */
            P = (seq_t)p;
        else            /* even, use 2*m-1 - p */
            P = (seq_t)(2 * m - 1 - (idx_t)p);
    }

    return base + P;
}
