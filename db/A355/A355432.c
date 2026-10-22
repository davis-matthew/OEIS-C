/*
 * A355432  a(n) = number of k < n such that rad(k) = rad(n) and k does not
 *          divide n, where rad(k) = A007947(k).
 * https://oeis.org/A355432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k < n such that rad(k) = rad(n) and k does not
 *            divide n
 * Range:     u8 n <= 585, u16 n <= 9809, u32 n <= 9809, u64 n <= 9809,
 *            u128 n <= 9809, big any n
 * Helpers:   dfs_smooth, factor_u64, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/rad.h"
#include "../utils/factor_u64.h"
#include "../utils/dfs_smooth.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t rad_n = rad(n);
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
