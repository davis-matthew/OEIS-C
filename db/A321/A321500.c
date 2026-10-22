/*
 * A321500  Triangular table T(n,k) = (n+k)*(n^2+k^2), n >= k >= 0; read by
 *          rows n = 0, 1, 2, ...
 * https://oeis.org/A321500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) with r = max{k | k(k+1)/2 <= n}, k = n - r(r+1)/2,
 *            * T(r,k) = (r+k)*(r^2 + k^2). That is, the triangular table *
 *            T(n,k) = (n+k)*(n^2 + k^2) read by rows
 * Range:     u8 n <= 13, u16 n <= 375, u32 n <= 9825, u64 n <= 9825,
 *            u128 n <= 9825, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, though idx_t is defined externally */

seq_t a(idx_t n)
{
    /* Find the row r.  r satisfies r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t low = 0;
    idx_t high = 1;

    /* Grow high until it exceeds n in the triangular bound. */
    while (((unsigned __int128)high * (high + 1)) / 2 <= n) {
        high <<= 1;          /* high grows geometrically; safe for u64 */
        if (high == 0) {     /* overflow, set to maximum */
            high = (idx_t)-1;
            break;
        }
    }

    /* Binary search for the greatest r with r(r+1)/2 <= n. */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);   /* mid rounded up */
        unsigned __int128 tri = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }

    idx_t r = low;
    /* Compute k = n - r(r+1)/2. */
    unsigned __int128 base128 = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t base = (idx_t)base128;          /* base <= n, fits in idx_t */
    idx_t k = n - base;

    /* Now compute T(r,k) = (r+k)*(r^2 + k^2). */
    seq_t r_seq = (seq_t)r;
    seq_t k_seq = (seq_t)k;
    seq_t r_sq   = r_seq * r_seq;
    seq_t k_sq   = k_seq * k_seq;
    seq_t sum_nk = r_seq + k_seq;

    return sum_nk * (r_sq + k_sq);
}
