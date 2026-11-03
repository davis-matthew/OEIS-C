/*
 * A355432  a(n) = number of k < n such that rad(k) = rad(n) and k does not
 *          divide n, where rad(k) = A007947(k).
 * https://oeis.org/A355432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k < n such that rad(k) = rad(n) and k does not
 *            divide n
 * Range:     u8 n <= 585, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 136 terms through n = 9809 (2026-11-01)
 * Helpers:   factor_u64, radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/radical.h"

/* Recursive helper: enumerate all t (1 <= t <= limit) formed from the primes in pr[]
 * and count both total and how many of them divide n_over_R. */
static void dfs_smooth(const uint64_t *pr, size_t cnt, size_t idx,
                       seq_t cur, seq_t limit, seq_t n_over_R,
                       seq_t *total, seq_t *divcnt)
{
    if (idx == cnt) {
        (*total)++;
        if (n_over_R % cur == 0)
            (*divcnt)++;
        return;
    }
    uint64_t p = pr[idx];
    seq_t val = cur;
    while (val <= limit) {
        dfs_smooth(pr, cnt, idx + 1, val, limit, n_over_R, total, divcnt);
        if (val > limit / (seq_t)p)
            break;
        val *= (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t rad_n = radical(n);
    idx_t R = (idx_t)rad_n;          /* rad(n) fits in idx_t for n <= 10⁴ */

    /* Unique prime factors of n */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);

    if (cnt == 0)                  /* only possible for n == 1 */
        return 0;

    /* Maximum multiplier t such that R * t < n */
    uint64_t max_t = ((idx_t)n - 1) / R;
    if (max_t == 0)
        return 0;

    seq_t n_over_R = (seq_t)(n / R);

    seq_t total = 0, divcnt = 0;
    dfs_smooth(pr, cnt, 0, (seq_t)1, (seq_t)max_t, n_over_R, &total, &divcnt);

    return total - divcnt;   /* exclude cases where k divides n */
}
