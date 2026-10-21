/*
 * A252665  Number of ways to write n as n = a*b*c*d*e with 1 <= a <= b <= c
 *          <= d <= e <= n.
 * https://oeis.org/A252665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5‑tuples (a,b,c,d,e) with * 1 ≤ a ≤ b ≤ c ≤ d ≤
 *            e ≤ n and a*b*c*d*e = n
 * Range:     u8 n <= 4319, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Integer square‑root via binary search.
 * Works for all idx_t ≤ 2^64‑1. */
static inline idx_t isqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= x / mid)   /* mid*mid ≤ x  (avoid overflow) */
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* n ≥ 1.  For safety we handle n = 0 in the same way. */
    if (n == 0) return 0;

    /* Collect all divisors of n in ascending order.  We store
     * the small divisors (< sqrt(n)) and the large ones separately
     * and then concatenate them.  The total number of divisors of
     * a 64‑bit integer below 10 000 never exceeds 200. */
    idx_t small[200], large[200];
    size_t s = 0, l = 0;
    idx_t limit = isqrt(n);
    for (idx_t i = 1; i <= limit; ++i) {
        if (n % i == 0) {
            small[s++] = i;
            idx_t j = n / i;
            if (j != i) large[l++] = j;
        }
    }

    idx_t divs[200];
    size_t dv = 0;
    for (size_t i = 0; i < s; ++i) divs[dv++] = small[i];
    for (size_t i = l; i > 0; --i) divs[dv++] = large[i - 1];

    seq_t count = 0;

    /* Enumerate all non‑decreasing tuples a,b,c,d,e.  Because the
     * divisors are sorted, we can stop each inner loop when the
     * current divisor exceeds the remaining cofactor. */
    for (size_t ia = 0; ia < dv; ++ia) {
        idx_t a_val = divs[ia];
        idx_t n1 = n / a_val;
        for (size_t ib = ia; ib < dv; ++ib) {
            idx_t b_val = divs[ib];
            if (b_val > n1) break;
            if (n1 % b_val != 0) continue;
            idx_t n2 = n1 / b_val;
            for (size_t ic = ib; ic < dv; ++ic) {
                idx_t c_val = divs[ic];
                if (c_val > n2) break;
                if (n2 % c_val != 0) continue;
                idx_t n3 = n2 / c_val;
                for (size_t id = ic; id < dv; ++id) {
                    idx_t d_val = divs[id];
                    if (d_val > n3) break;
                    if (n3 % d_val != 0) continue;
                    idx_t e_val = n3 / d_val;
                    if (e_val < d_val) continue;   /* enforce non‑decreasing order */
                    ++count;
                }
            }
        }
    }

    return count;
}
