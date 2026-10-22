/*
 * A399692  Triangle T(n,k) read by rows of Knuth's deltas for comparison
 *          trees used in a uniform binary search.
 * https://oeis.org/A399692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, new
 * Formula:   a(n) = T(r,k) of Knuth's triangle where r is the row containing
 *            position n and k is the column index in that row, T(r,k) =
 *            floor((r + 2^(k-1)) / 2^k). The sequence a(n) is the triangle
 *            read row‑by‑row
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* auxiliary: floor of base‑2 logarithm of a 64‑bit unsigned integer
   (argument is guaranteed to be >0). */
static inline idx_t floor_log2_u64(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* compute the a(n) value as defined in OEIS A399692 */
seq_t a(idx_t n)
{
    /* n is 1‑based; the following routine works for all n >= 1   */
    idx_t r = 1;           /* current row number          */
    idx_t cum = 0;         /* number of entries before row r   */
    idx_t len;             /* length of current row      */

    /* locate the row that contains the n‑th element */
    while (1)
    {
        len = floor_log2_u64(r) + 2;   /* entries in row r */
        if (cum + len >= n)
            break;                     /* row r contains n */
        cum += len;
        ++r;
    }

    /* column (1‑based) inside the row */
    idx_t k = n - cum;   /* 1 <= k <= len */

    /* T(r,k) = floor((r + 2^(k-1)) / 2^k) .
       Compute with 128‑bit arithmetic to avoid overflow when seq_t is 64‑bit. */
    unsigned __int128 tmp = (unsigned __int128)r
                          + ((unsigned __int128)1 << (k - 1));
    return (seq_t)(tmp >> k);
}
