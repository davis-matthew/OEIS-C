/*
 * A273323  a(n) = greatest number k <= n^2 having exactly n divisors, or 0 if
 *          no such k exists.
 * https://oeis.org/A273323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest integer k ≤ n² that has exactly n divisors, or 0
 *            if no such k exists
 * Range:     u8 n <= 15, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Static state to cache divisor counts up to 36² = 1296. */
static struct {
    bool initialized;
    seq_t divcnt[1297];
} spf_cache = { .initialized = false };

/* Precompute divisor counts for all numbers up to 1296. */
static void init_divcnt(void) {
    if (spf_cache.initialized)
        return;
    for (idx_t num = 1; num <= 1296; ++num) {
        seq_t cnt = 0;
        for (idx_t d = 1; d * d <= num; ++d) {
            if (num % d == 0) {
                ++cnt;
                if (d != num / d)
                    ++cnt;
            }
        }
        spf_cache.divcnt[num] = cnt;
    }
    spf_cache.initialized = true;
}

seq_t a(idx_t n) {
    /* For n = 0 (outside OEIS offset) return 0. */
    if (n == 0)
        return 0;

    /* For n > 36, the minimal number with n divisors exceeds n².
       Return 0 according to the OEIS comment. */
    if (n > 36)
        return 0;

    init_divcnt();

    /* Compute n² in seq_t. */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    idx_t limit = (idx_t)n_sq;  /* safe: n² ≤ 1296 */

    /* Search downwards for the largest k with exactly n divisors. */
    for (idx_t k = limit; k >= 1; --k) {
        if (spf_cache.divcnt[k] == n)
            return (seq_t)k;
    }

    /* No such number found. */
    return 0;
}
