/*
 * A394704  Triangle read by rows: coefficients of reciprocal polynomials
 *          refining A013499.
 * https://oeis.org/A394704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 42, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binom, cumulative_rows
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the nth coefficient of the flattened triangle whose r‑th
 * row (r = 0,1,2,…) consists of the n+2 coefficients of the numerator of
 * the generating function
 *
 *    Σ_{k≥0} p_r(k) t^k  =  A_r(t) / (1 - t)^{r+2}
 *
 * where the integer‑valued polynomial is
 *
 *    p_r(x) = (∏_{i=1..r} ((r+1)*x + i)) * (2x + 1) / r!.
 *
 * The coefficient a(n) is therefore the (r+2)‑fold forward differences of
 * the sequence p_r(k) evaluated at k = 0…pos, taken at the pos‑th element.
 *
 * That is, let s_k = p_r(k) for k = 0…pos.  Compute
 *
 *    f = s
 *    repeat r+2 times:
 *        for k = pos down to 1:
 *            f[k] -= f[k-1]
 *
 * Then a(n) = f[pos].
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/cumulative_rows.h"

static inline void locate_row(idx_t n, idx_t *r, idx_t *pos)
{
    /* find smallest r with cumulative_rows(r) > n */
    idx_t low = 0, high = 1;
    while (cumulative_rows(high) <= n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cumulative_rows(mid) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    /* now low == high == first r with cumulative_rows(r) > n  */
    *r = low;
    *pos = n - ((low == 0) ? 0 : cumulative_rows(low - 1));
}

seq_t a(idx_t n)
{
    idx_t r, pos;
    locate_row(n, &r, &pos);

    /* allocate array of size pos+1 */
    size_t m = (size_t)pos;
    seq_t *f = (seq_t *)calloc(m + 1, sizeof(seq_t));
    if (!f)
        return (seq_t)0;   /* out of memory */

    /* fill f[k] = p_r(k) = binom((r+1)*k + r, r) * (2*k + 1) */
    for (size_t k = 0; k <= m; ++k) {
        idx_t a_val = (r + 1) * (idx_t)k + r;
        seq_t val = binom(a_val, r);
        val *= (seq_t)(2 * k + 1);
        f[k] = val;
    }

    /* perform (r+2) forward differences in place */
    for (idx_t step = 0; step < r + 2; ++step) {
        for (size_t k = m; k > 0; --k)
            f[k] -= f[k - 1];
    }

    seq_t result = f[m];
    free(f);
    return result;
}
