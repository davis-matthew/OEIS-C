/*
 * A368874  a(n) is the number of points with integer coordinates located
 *          strictly between the parabolas y = n - x^2 and y = x^2 - n.
 * https://oeis.org/A368874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊√(n-1)⌋} (2*(n - k²) - 1)·(k==0?1:2)
 * Range:     u8 n <= 21, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  = 2·Σ_{k=0}^{⌊√(n-1)⌋} (n - k²)  - (⌊√(n-1)⌋ + 1)                     */

#include <stdint.h>

/* Integer floor square root of a 64‑bit unsigned integer.
   Uses only integer arithmetic. */
static idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32);     /* hi > sqrt(UINT64_MAX)   */
    idx_t ans = 0;

    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 prod = (unsigned __int128)mid * mid;   /* safe multiply */

        if (prod <= v) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* offset 1, but guard for completeness */

    /* maximum |x| such that x² ≤ n-1 */
    idx_t max_x = isqrt_u64(n - (idx_t)1);

    /* start with contribution from x = 0 : 2·n - 1 */
    seq_t N = (seq_t)n;
    seq_t ans = (seq_t)(2 * N - (seq_t)1);

    /* add contributions for x = ±k, 1 ≤ k ≤ max_x */
    for (idx_t k = (idx_t)1; k <= max_x; ++k) {
        idx_t ksq = k * k;               /* k² fits in idx_t (uint64_t) */
        seq_t delta = N - (seq_t)ksq;    /* always ≥ 1 because k² ≤ n-1 */
        seq_t val = (seq_t)(2 * delta - (seq_t)1);
        ans += val * (seq_t)2;           /* twice for +k and –k */
    }

    return ans;
}
