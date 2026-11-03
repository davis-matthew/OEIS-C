/*
 * A242641  Array read by antidiagonals upwards: B(s,n) ( s>=1, n >= 0) =
 *          number of s-line partitions of n.
 * https://oeis.org/A242641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the 2‑D array B(s,n) of s‑line plane
 *            partitions * along antidiagonals upwards. B(s,n) is the number
 *            of s‑line partitions of n, * i.e. the coefficient of q^n in *
 *            ∏_{i=1..s} (1−q^i)^(−i) · ∏_{j≥s+1} (1−q^j)^(−s). * The
 *            implementation first computes the pair (s,n) from t...
 * Range:     u8 n <= 88, u16 n <= 346, u32 n <= 1645, u64 n <= 9169,
 *            u128 n <= 20100, big any n
 * Validated: standard, pass, 107 terms through n = 9058 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t compute_B(idx_t s, idx_t ncol)
{
    if (ncol == 0) return (seq_t)1;

    size_t len = (size_t)ncol + 1;
    seq_t *dp    = mem_new(seq_t, len);
    seq_t *newdp = mem_new(seq_t, len);
    if (!dp || !newdp) {
        mem_free(dp); mem_free(newdp);
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;

    for (idx_t part = 1; part <= ncol; ++part) {
        idx_t exp  = part <= s ? part : s;
        idx_t maxH = ncol / part;

        mem_set(newdp, 0, len * sizeof(seq_t));

        seq_t coef = (seq_t)1;
        for (idx_t h = 0; h <= maxH; ++h) {
            size_t shift = (size_t)h * (size_t)part;
            if (h > 0) {
                coef = coef * (seq_t)(exp - 1 + h);
                coef = coef / (seq_t)h;   /* exact division */
            }

            for (size_t t = shift; t < len; ++t) {
                newdp[t] += dp[t - shift] * coef;
            }
        }
        mem_copy(dp, newdp, len * sizeof(seq_t));
    }

    seq_t result = dp[ncol];
    mem_free(dp); mem_free(newdp);
    return result;
}

static void antidiagonal_coords(idx_t n, idx_t *s, idx_t *c)
{
    idx_t idx = n;               /* 1‑based index */
    idx_t low = 0, high = 1;
    while (high * (high + 1) / 2 < idx) high *= 2;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= idx) high = mid;
        else                              low = mid;
    }
    idx_t t      = high;                       /* diagonal number (s + n)   */
    idx_t prev   = (t - 1) * t / 2;            /* elements before this diag */
    idx_t offset = idx - prev - 1;             /* 0‑based offset in diag   */
    *s = t - offset;    /* row index (starts at 1)   */
    *c = offset;        /* column index (starts at 0) */
}

seq_t a(idx_t n)
{
    idx_t s, col;
    antidiagonal_coords(n, &s, &col);
    return compute_B(s, col);
}
