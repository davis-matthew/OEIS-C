/*
 * A344646  Array read by antidiagonals T(n,k) = ((n+k+1)^2 - (n+k+1) mod 2)/4
 *          + min(n,k) for n and k >= 0.
 * https://oeis.org/A344646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inevitably uses unsigned __int128 for safe intermediate arithmetic.
   Computes OEIS A344646: array read by anti‑diagonals
   with T(n,k) = ((n+k+1)^2 - (n+k+1) mod 2)/4 + min(n,k). */
#include <stdint.h>

/* Find the diagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2
   by a simple binary search using 128‑bit arithmetic. */
static inline idx_t _diag_of(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* Grow high until it exceeds n */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        if (high > UINT64_MAX / 2) { high = UINT64_MAX; break; }
        high <<= 1;
    }

    /* Binary search for the largest s with T(s) <= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* Determine diagonal and coordinates (i,j) with i = nArg, j = kArg */
    const idx_t s = _diag_of(n);
    const idx_t start = (idx_t)((unsigned __int128)s * (s + 1) / 2);
    const idx_t t = n - start;            /* 0 <= t <= s */
    const idx_t nArg = t;
    const idx_t kArg = s - t;

    /* m = nArg + kArg + 1 = s + 1 */
    const idx_t m = nArg + kArg + 1;
    const idx_t lo = m >> 1;           /* floor(m/2) */
    const idx_t hi = m - lo;           /* ceil(m/2)  */
    const idx_t minnk = nArg < kArg ? nArg : kArg;

    /* term = floor(m/2) * ceil(m/2) + min(n,k) */
    const seq_t prod = (seq_t)((unsigned __int128)lo * hi);
    return prod + (seq_t)minnk;
}
