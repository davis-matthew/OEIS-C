/*
 * A309559  Triangle read by rows: T(n,k) = 1 + n + k^2/2 - k/2 + k*(n-k), n
 *          >= 0, 0 <= k <= n.
 * https://oeis.org/A309559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r = floor((sqrt(8*n + 1) - 1)/2), k = n - r*(r+1)/2. Then
 *            a(n) = 1 + r + k*(k-1)/2 + k*(r-k). This gives the row‑wise
 *            flattened values of the triangle T(n,k) = 1 + n + k^2/2 - k/2 +
 *            k*(n-k)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer square root of an unsigned 128‑bit integer.
   Returns floor(sqrt(x)).  Uses a simple binary search. */
static idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;            /* 2^32 is large enough for unsigned __int128 sqrt */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Compute a(n) for the OEIS sequence A309559. */
seq_t a(idx_t n)
{
    /* r = floor((sqrt(8*n + 1) - 1)/2) */
    unsigned __int128 d = (unsigned __int128)1 + (unsigned __int128)8 * n;
    idx_t r = (isqrt_u128(d) - 1) / 2;

    /* k = n - r*(r+1)/2 */
    idx_t prefix = r * ((idx_t)r + 1) / 2;
    idx_t k = n - prefix;

    /* Compute T = 1 + r + k*(k-1)/2 + k*(r-k) */
    seq_t r_seq = (seq_t)r;
    seq_t k_seq = (seq_t)k;
    seq_t term = (seq_t)1 + r_seq + k_seq * (k_seq - (seq_t)1) / (seq_t)2
                 + k_seq * (r_seq - k_seq);

    return term;
}
