/*
 * A357734  Array T(n,k), read by descending antidiagonals, whose rows are
 *          numbers congruent to p or q mod r, with 0 <= p < q < r, sorted by
 *          r, then p, then q.
 * https://oeis.org/A357734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = value of the (m-i)th element (k) in the row of numbers
 *            congruent to p or q modulo r, where the row (r,p,q) is obtained
 *            from the antidiagonal index i derived from n; row values are
 *            given by a(k) = (2*r*k + 2*p + 2*q - 3*r - (2*p - 2*q +
 *            r)*(-1)^k)/4
 * Range:     u8 n <= 5691, u16 n <= 9316, u32 n <= 9316, u64 n <= 9316,
 *            u128 n <= 9316, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Compute the antidiagonal index m such that
       m*(m-1)/2 >= n.  Binary search using 128‑bit
       arithmetic to avoid overflow. */
    idx_t low = 2, high = 2;
    while (1) {
        __int128 val = (__int128)high * (high - 1) / 2;
        if (val >= n) break;
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 val = (__int128)mid * (mid - 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Number of elements before this antidiagonal. */
    __int128 prev128 = (__int128)(m - 1) * (m - 2) / 2;
    idx_t prev = (idx_t)prev128;

    /* Row index (i) and column index (k) in the table. */
    idx_t row_idx = n - prev;          /* 1‑based */
    idx_t k = m - row_idx;             /* 1‑based column */

    /* Determine the row parameters (r,p,q) for the given row index. */
    /* Find smallest r such that cumulative rows S(r) = r(r^2-1)/6 >= row_idx. */
    idx_t lowr = 2, highr = 2;
    while (1) {
        __int128 s = (__int128)highr * ( (__int128)highr * highr - 1 ) / 6;
        if (s >= row_idx) break;
        highr <<= 1;
    }
    while (lowr < highr) {
        idx_t mid = lowr + (highr - lowr) / 2;
        __int128 s = (__int128)mid * ((__int128)mid * mid - 1) / 6;
        if (s >= row_idx)
            highr = mid;
        else
            lowr = mid + 1;
    }
    idx_t r = lowr;

    /* Total rows before this r. */
    __int128 s_prev128 = (__int128)(r - 1) * ( (__int128)(r - 1) * (r - 1) - 1 ) / 6;
    idx_t s_prev = (idx_t)s_prev128;

    /* Offset within the block for this r. */
    idx_t offset_in_block = row_idx - s_prev - 1;  /* 0‑based */

    /* Find p (and q) from the offset. */
    idx_t cumulative = 0;
    idx_t p = 0;
    for (; p < r - 1; ++p) {
        idx_t block_size = r - p - 1;
        if (offset_in_block < cumulative + block_size)
            break;
        cumulative += block_size;
    }
    idx_t q = p + 1 + (offset_in_block - cumulative);

    /* Compute a(k) using the closed‑form formula. */
    __int128 two_r_k      = (__int128)2 * (__int128)r * (__int128)k;
    __int128 two_p_plus_q = (__int128)2 * (__int128)p + (__int128)2 * (__int128)q;
    __int128 minus3r      = (__int128)-3 * (__int128)r;
    __int128 coeff        = (__int128)2 * (__int128)p - (__int128)2 * (__int128)q + (__int128)r;
    int    sign           = (k & 1) ? -1 : 1;          /* (-1)^k */
    __int128 numerator    = two_r_k + two_p_plus_q + minus3r - coeff * sign;
    seq_t result          = (seq_t)(numerator / 4);
    return result;
}
