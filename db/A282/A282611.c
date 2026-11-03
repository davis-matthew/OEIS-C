/*
 * A282611  Expansion of q^(-1/3) * c(q) * c(q^3) / 9 in powers of q where c()
 *          is a cubic AGM theta function.
 * https://oeis.org/A282611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^(n-1) in ∏_{d≥1} (1 - q^d)^{c_d}, with
 *            c_d = -1 + 2·[d % 3 == 0] + 3·[d % 9 == 0] (period‑9 pattern)
 * Range:     u8 n <= 20, u16 n <= 706, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 86 terms through n = 4968 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static seq_t *coeff = NULL;      /* coeff[k] = coefficient of q^k in G(q) */
static size_t coeff_size = 0;    /* number of elements in coeff[] */

static int64_t *S = NULL;         /* S[k] = Σ_{d|k} d * c_d */
static size_t S_size = 0;         /* number of elements in S[]   */

/* grow the coefficient array and the divisor–sum array up to "need" elements   */
static void ensure_size(idx_t need)
{
    if (need <= coeff_size) return;            /* already large enough */

    /* remember old size before resizing */
    idx_t old_coeff_size = coeff_size;
    idx_t old_S_size      = S_size;

    /* resize coefficient array */
    seq_t *new_coeff = mem_resize(seq_t, coeff, need);
    if (!new_coeff && need > 0) { exit(1); }
    coeff = new_coeff;
    coeff_size = need;

    /* resize divisor–sum array */
    int64_t *new_S = (int64_t *)realloc(S, need * sizeof(int64_t));
    if (!new_S && need > 0) { exit(1); }
    S = new_S;
    S_size = need;

    /* clear new entries of S */
    for (idx_t i = 0; i < need; ++i) S[i] = 0;

    /* compute c_d and accumulate S[d] */
    for (idx_t d = 1; d < need; ++d) {
        int c_d = -1;
        if (d % 3 == 0) c_d += 2;
        if (d % 9 == 0) c_d += 3;
        for (idx_t m = d; m < need; m += d) {
            S[m] += (int64_t)d * c_d;
        }
    }

    /* set initial coefficient coeff[0] = 1 */
    if (need > 0) coeff[0] = (seq_t)1;

    /* compute new coefficients using the recurrence */
    for (idx_t n = old_coeff_size; n < need; ++n) {
        /* n must be >=1 because coeff[0] already handled */
        if (n == 0) continue;
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (idx_t k = 1; k <= n; ++k) {
            int64_t sk = S[k];
            if (sk > 0) {
                sum_pos += mul_small(coeff[n - k], (unsigned int)sk);
            } else if (sk < 0) {
                sum_neg += mul_small(coeff[n - k], (unsigned int)(-sk));
            }
        }
        seq_t diff = sum_neg - sum_pos;          /* diff >= 0 by construction */
        coeff[n] = diff / (seq_t)n;              /* integer division, exact */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    /* we need coeff[n-1], thus at least n coefficients are required */
    ensure_size(n);
    return coeff[n - 1];
}
