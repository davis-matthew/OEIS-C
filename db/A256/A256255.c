/*
 * A256255  Triangle read by rows: T(n,k) = 6*k + 1, n>=0, 0<=k<=(2^n-1).
 * https://oeis.org/A256255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 6 * k + 1 where * r = floor(log₂(n + 1)) (0‑based row
 *            number) * k = n – (2ʳ – 1) (column index within that row) * Thus
 *            a(n) = 6 * (n – (2ʳ – 1)) + 1
 * Range:     u8 n <= 105, u16 n <= 9994, u32 n <= 9994, u64 n <= 9994,
 *            u128 n <= 9994, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for UINT64_MAX */

/* Compute floor(log₂(x)) for 0 < x <= 2⁶⁴.
 * The function is safe for the maximum value that fits in a 64‑bit unsigned integer. */
static unsigned floor_log2_uint64(idx_t x)
{
    unsigned r = 0;
    while (x >>= 1) ++r;   /* shift until x becomes 0, counting iterations */
    return r;
}

seq_t a(idx_t n)
{
    /* Determine the row number r.  For n = 2⁶⁴−1 the value of n+1 would
     * overflow uint64_t; we treat that as the special case where r = 63. */
    unsigned r;
    if (n == UINT64_MAX) {
        r = 63;                     /* 2⁶³ <= n+1 <= 2⁶⁴  */
    } else {
        r = floor_log2_uint64((idx_t)1 + n);
    }

    /* Start index of the row:  start = 2ʳ – 1 */
    idx_t start = ((idx_t)1 << r) - (idx_t)1;

    /* Column index within the row */
    idx_t k = n - start;

    /* Compute the value: 6 * k + 1 */
    return (seq_t)6 * (seq_t)k + (seq_t)1;
}
