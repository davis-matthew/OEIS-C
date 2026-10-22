/*
 * A320134  Number of integer solutions to a^2 + b^2 + 2*c^2 + 12*d^2 = n.
 * https://oeis.org/A320134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (a,b,c,d) ∈ ℤ⁴ : a² + b² + 2c² + 12d² = n }
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A320134) */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root for unsigned 64‑bit values, using binary search
 * and 128‑bit arithmetic to avoid overflow for the intermediate product.
 */
static idx_t isqrt_idx(idx_t n)
{
    if (n == 0) return 0;
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return low - 1;   /* floor(sqrt(n)) */
}

/* Count integer solutions to a² + b² + 2c² + 12d² = n.
 * Uses an auxiliary array ab[s] = #{ (a,b) : a² + b² = s } for s ≤ n,
 * then sums over all (c,d) pairs.
 */
seq_t a(idx_t n)
{
    /* Allocate array for a² + b² counts */
    seq_t *ab = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!ab) return (seq_t)0;   /* Out of memory – return 0 (adjustable) */

    /* Precompute counts of a² + b² up to n */
    idx_t max_ab = isqrt_idx(n);
    for (int64_t a = -(int64_t)max_ab; a <= (int64_t)max_ab; ++a) {
        idx_t a2 = (idx_t)(a * a);
        for (int64_t b = -(int64_t)max_ab; b <= (int64_t)max_ab; ++b) {
            idx_t s = a2 + (idx_t)(b * b);
            if (s <= n) ++ab[s];
        }
    }

    /* Sum over all (c,d) pairs. */
    seq_t total = 0;
    idx_t max_c = isqrt_idx(n / 2);
    for (int64_t c = -(int64_t)max_c; c <= (int64_t)max_c; ++c) {
        idx_t termC = 2 * (idx_t)(c * c);
        if (termC > n) continue;   /* safety */
        idx_t rem1 = n - termC;
        idx_t max_d = isqrt_idx(rem1 / 12);
        for (int64_t d = -(int64_t)max_d; d <= (int64_t)max_d; ++d) {
            idx_t termD = 12 * (idx_t)(d * d);
            idx_t rem2 = rem1 - termD;
            /* rem2 is guaranteed ≤ n */
            total += ab[rem2];
        }
    }

    free(ab);
    return total;
}
