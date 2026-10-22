/*
 * A334594  Irregular table read by rows: T(n,k) is the binary interpretation
 *          of the k-th row of the XOR-triangle with first row generated from
 *          the binary expansion of n. 1 <= k <= A070939(n).
 * https://oeis.org/A334594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf, look
 * Formula:   not stated
 * Range:     u8 n <= 1793, u16 n <= 9217, u32 n <= 9217, u64 n <= 9217,
 *            u128 n <= 9217, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  
 * a(n) = T(m,k) where
 *   - m is the smallest integer such that sum_{i=1}^{m} bitlen(i) ≥ n,
 *   - k = n - sum_{i=1}^{m-1} bitlen(i) - 1,
 *   - T(m,k) is the integer obtained by interpreting the k‑th row of the
 *     XOR‑triangle whose first row is the binary representation of m.
 *   The k‑th row is computed by iterating k times the transformation:
 *     next = (cur ^ (cur >> 1)) & mask,
 *   where cur is the current row as an integer and `mask` keeps only the
 *   lower (current length‑1) bits.
 */

#include <stdint.h>

static inline idx_t bitlen_u64(idx_t x)
{
    /* x is guaranteed > 0 */
    return 64 - __builtin_clzll(x);
}

seq_t a(idx_t n)
{
    /* Locate the triangle index m and row offset k */
    idx_t total = 0;
    idx_t m_minus1 = 0;          /* number of completed triangles */
    while (total + bitlen_u64(m_minus1 + 1) < n)
    {
        total += bitlen_u64(m_minus1 + 1);
        ++m_minus1;
    }
    idx_t m = m_minus1 + 1;      /* current triangle number (≥1) */
    idx_t k = n - total - 1;     /* row offset within this triangle */

    /* Compute the value of the k‑th row of triangle m */
    seq_t value = (seq_t)m;
    idx_t initial_len = bitlen_u64(m);    /* length of the first row */

    for (idx_t step = 0; step < k; ++step)
    {
        idx_t cur_len = initial_len - step;   /* current row length */
        /* mask = lower (cur_len-1) bits set to 1 */
        seq_t mask;
        if (cur_len <= 1)
            mask = 0;
        else
            mask = (seq_t)(((idx_t)1 << (cur_len - 1)) - 1);
        value = (value ^ (value >> 1)) & mask;
    }

    return value;
}
