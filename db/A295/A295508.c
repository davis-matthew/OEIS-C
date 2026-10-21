/*
 * A295508  Triangle read by rows, related to binary partitions of n.
 * https://oeis.org/A295508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) equals the T(r,k) entry of the triangle described in OEIS
 *            A295508: T(n,0)=n, T(n,k)=n-2^(k-1) for k>0, with rows read in
 *            row‑major order. This implementation finds the row r containing
 *            index n by a linear search and returns the appropriate T(r,k)
 *            value
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned floor_log2_u64(idx_t v)
{
    return (v == 0) ? 0 : 63 - __builtin_clzll(v);
}

seq_t a(idx_t n)
{
    idx_t cumulative = 0;   /* number of elements seen so far */
    idx_t r = 0;            /* current row */

    /* Find the row r such that cumulative + len(r) > n */
    for (;; ++r)
    {
        idx_t len;
        if (r == 0)
            len = 1;                       /* row 0: single element */
        else
            len = floor_log2_u64(r) + 2;   /* len(r) = floor_log2(r)+2 */

        if (n - cumulative < len)          /* n < cumulative + len */
            break;

        cumulative += len;
    }

    idx_t k = n - cumulative;               /* column index within row r */

    if (k == 0)
    {
        return (seq_t)r;                       /* T(r,0) = r */
    }
    else
    {
        /* Compute 2^(k-1) as a seq_t; k-1 <= 63  */
        seq_t pow2 = (seq_t)(1ULL << (k - 1));
        return (seq_t)r - pow2;                /* T(r,k) = r - 2^(k-1) */
    }
}
