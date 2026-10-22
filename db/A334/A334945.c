/*
 * A334945  Irregular triangle read by rows: T(n,k), n >= 1, k >= 1, in which
 *          column k lists successive blocks of k consecutive integers that
 *          differ by 3, where the m-th block starts with m, m >= 1, and the
 *          first element of column k is in the row that is the k-th
 *          pentagonal number (A000326).
 * https://oeis.org/A334945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Entries T(n,k) are defined as described in the OEIS entry; a(n)
 *            is the sequence obtained by reading the rows of the triangle
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A334945 via irregular triangle read by rows. */

#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for non‑positive indices */

    idx_t r = 0;                     /* current row number */
    idx_t cumulative = 0;            /* total number of entries up to current row */
    idx_t kmax = 0;                  /* number of entries in current row */

    /* Find the row that contains the n-th element */
    while (1) {
        ++r;
        idx_t sqrt_term = int_sqrt((idx_t)1 + (idx_t)24 * r);
        kmax = ((idx_t)1 + sqrt_term) / (idx_t)6;   /* floor((1+sqrt(1+24r))/6) */
        cumulative += kmax;
        if (cumulative >= n) break;
    }

    idx_t cumulative_prev = cumulative - kmax;     /* entries before this row */
    idx_t pos = n - cumulative_prev;               /* 1‑based position in this row */
    idx_t k = pos;                                 /* column number */

    /* Compute the value T(r,k) */
    idx_t Pk = k * (3 * k - 1) / 2;                 /* pentagonal number P(k) */
    idx_t t   = r - Pk;                             /* offset within block */
    idx_t m   = t / k + 1;                          /* block number */
    idx_t i   = t % k;                              /* index within block (0‑based) */

    seq_t value = (seq_t)m + (seq_t)3 * (seq_t)i;
    return value;
}
