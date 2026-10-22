/*
 * A350900  Triangle read by rows: T(n, k) = Sum_{i=1..n} gcd(i,n) /
 *          gcd(gcd(i,k),n) for 1 <= k <= n.
 * https://oeis.org/A350900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the n‑th entry of the triangle read by rows. * Triangle
 *            entries are defined by * T(r,c) = Σ_{i=1..r} gcd(i,r) /
 *            gcd(gcd(i,c),r) * and a(n) is obtained by flattening the
 *            triangle row by row
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the row r such that the total number of entries before row r
     * is less than n and the total up to row r is at least n.  Use
     * a binary search on the triangular numbers to avoid overflow. */
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column k within the row: k = n - entries_before_row */
    unsigned __int128 before = (unsigned __int128)(r - 1) * r / 2;
    idx_t c = (idx_t)(n - before);

    /* Compute T(r,c) directly. */
    seq_t acc = 0;
    seq_t seq_r = (seq_t)r;
    seq_t seq_c = (seq_t)c;
    for (idx_t i = 1; i <= r; ++i) {
        seq_t g_ir = gcd((seq_t)i, seq_r);       /* gcd(i,r)          */
        seq_t g_ic = gcd((seq_t)i, seq_c);       /* gcd(i,c)          */
        seq_t denom = gcd(g_ic, seq_r);          /* gcd(gcd(i,c),r)   */
        acc += g_ir / denom;                     /* add the term      */
    }
    return acc;
}
