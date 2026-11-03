/*
 * A245717  Triangle read by rows: T(n,k) = gcd(n,k^2), 1 <= k <= n.
 * https://oeis.org/A245717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = gcd(r, k^2) where r = ceil((sqrt(8n+1)-1)/2) and k = n -
 *            r(r-1)/2 (indices start at 1)
 * Range:     u8 n <= 7, u16 n <= 1987, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Validated: standard, pass, 107 terms through n = 6881 (2026-11-01)
 * Helpers:   gcd, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* safety, though a(0) not used */

    /* Convert the index to seq_t for arithmetic. */
    seq_t n_seq = (seq_t)n;

    /* r = ceil((sqrt(8n+1)-1)/2) */
    seq_t sqrt_val = floor_sqrt((seq_t)1 + (seq_t)8 * n_seq);
    seq_t r = (sqrt_val - 1) / 2;

    /* Adjust r if the triangular number is still too small. */
    seq_t t_r = r * (r + 1) / 2;
    if (t_r < n_seq)
        r++;

    /* k = n - r(r-1)/2 */
    seq_t t_prev = (r - 1) * r / 2;
    seq_t k = n_seq - t_prev;

    /* Compute gcd(r, k^2).  k^2 fits in seq_t for all realistic n. */
    seq_t k_sq = k * k;

    return gcd((seq_t)r, k_sq);
}
