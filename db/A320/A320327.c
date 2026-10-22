/*
 * A320327  Triangle T(n,m) = C(2*n,m)*C(2*n-1,n), 0 <= m <= 2*n, n >= 0.
 * https://oeis.org/A320327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = C(2*⌊√n⌋, n - ⌊√n⌋²) * C(2*⌊√n⌋ - 1, ⌊√n⌋)
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This gives the row‑major enumeration of the triangle
   T(n,m) = C(2n,m) * C(2n-1,n), 0≤m≤2n.                                   */

#include <stdint.h>

/* Compute floor(sqrt(n)) safely using binary search. */
static inline idx_t _floor_isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        /* avoid overflow: mid*mid <= n iff mid <= n/mid (for mid>0) */
        if (mid != 0 && mid > n / mid) {
            high = mid - 1;
        } else if (mid * mid <= n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

/* Compute binomial(n,k) for idx_t arguments; result in seq_t. */
static inline seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - i + 1) / (seq_t)i;
    }
    return res;
}

/* Return the a(n) term of sequence A320327. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* T(0,0) = 1 */

    idx_t r = _floor_isqrt_u64(n);          /* row index */
    idx_t m = n - r * r;                    /* column index within the row */

    /* Binomial factor independent of m. */
    seq_t factor = binom_calc(2 * r - 1, r);

    /* First factor depends on m. */
    seq_t binom_m = binom_calc(2 * r, m);

    return binom_m * factor;
}
