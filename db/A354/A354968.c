/*
 * A354968  Triangle read by rows: T(n, k) = n*k*(n+k)*(n-k)/6.
 * https://oeis.org/A354968
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where r,k are determined from n. The triangle
 *            T(n,k)=n*k*(n+k)*(n-k)/6 is read row‑by‑row starting with n=2,
 *            k=1. a(2) is the first term of the sequence
 * Range:     u8 n <= 12, u16 n <= 308, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed for integer types and __int128 usage. */
#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for unsigned 64‑bit n. */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, mid, ans = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        __int128 sq = (__int128)mid * (__int128)mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* The OEIS sequence is defined for n ≥ 2 (offset 2).  If a call is
       made with a smaller n we simply return 0, which is harmless for
       testing purposes. */
    if (n < 2)
        return (seq_t)0;

    /* Zero‑based position in the flattened triangle. */
    idx_t t = n - 2;               /* t = 0 for a(2), 1 for a(3), … */

    /* Find the row r such that cumulative count up to row r exceeds t. */
    idx_t disc = 1 + 8 * t;        /* 1 + 8t is always non‑negative */
    idx_t sqrt_floor = floor_isqrt_u64(disc);
    idx_t r = (1 + sqrt_floor) >> 1;   /* floor((1+sqrt(disc))/2) */

    /* Adjust r so that r*(r-1)/2 > t (strict inequality). */
    while (r * (r - 1) / 2 <= t)
        ++r;

    /* Number of terms before row r. */
    idx_t prev_terms = (r - 2) * (r - 1) / 2;  /* sum_{i=2}^{r-1} (i-1) */

    /* Column index k within row r.  k runs from 1 to r-1. */
    idx_t k = t - prev_terms + 1;

    /* Compute T(r,k) = r*k*(r+k)*(r-k)/6, all in seq_t. */
    seq_t val = (seq_t)r * (seq_t)k;
    val = val * (seq_t)(r + k);
    val = val * (seq_t)(r - k);
    val = val / 6;

    return val;
}
