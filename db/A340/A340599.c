/*
 * A340599  Number of factorizations of n into factors > 1 with length and
 *          greatest factor equal.
 * https://oeis.org/A340599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of factorizations of n into factors > 1 where the
 *            number of factors equals the greatest factor
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 136 terms through n = 9947 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t count_exact(idx_t L, idx_t n, idx_t min_f, idx_t max_allowed)
{
    /* Recursively count factorizations of n into L factors,
       each ≥ min_f and ≤ max_allowed, in non‑decreasing order. */
    if (L == 1) {
        return (min_f <= n && n <= max_allowed) ? 1 : 0;
    }
    seq_t total = 0;
    for (idx_t d = min_f; d <= max_allowed && d <= n; ++d) {
        if (n % d != 0) continue;
        idx_t rem = n / d;
        total += count_exact(L - 1, rem, d, max_allowed);
    }
    return total;
}

seq_t a(idx_t n)
{
    /* Sum over all possible lengths L with 2 ≤ L and 2^L ≤ n. */
    seq_t result = 0;
    for (idx_t L = 2; ; ++L) {
        if (((idx_t)1 << L) > n) break;
        seq_t cnt_max = count_exact(L, n, 2, L);        /* max factor ≤ L   */
        seq_t cnt_less = count_exact(L, n, 2, L - 1);   /* max factor ≤ L-1 */
        result += cnt_max - cnt_less;
    }
    return result;
}
