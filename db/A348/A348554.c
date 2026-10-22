/*
 * A348554  Irregular triangle read by rows: row n gives the divisors d of 2*n
 *          with 1 < d < 2*n, for n >= 2.
 * https://oeis.org/A348554
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the element at zero‑based position (n‑2) in the
 *            concatenation of the divisor lists of 2*m for m ≥ 2, with each
 *            list sorted ascending and excluding 1 and 2*m
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   divisor_count_u64, factor_u64, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/seq_cmp.h"

static void gen_divs_rec(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                         size_t idx, seq_t cur, seq_t *out, size_t *out_cnt,
                         seq_t limit)
{
    if (idx == cnt) {
        if (cur != (seq_t)1 && cur != limit)
            out[(*out_cnt)++] = cur;
        return;
    }
    seq_t val = cur;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divs_rec(pr, ex, cnt, idx + 1, val, out, out_cnt, limit);
        val *= (seq_t)pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* Undefined for n < 2 */

    /* Zero‑based index within the full concatenated sequence. */
    idx_t pos0 = n - 2;

    /* Find the row that contains this position. */
    idx_t row = 2;
    idx_t cum = 0;    /* Sum of lengths of rows 2 .. row‑1 */

    while (1) {
        idx_t len = divisor_count_u64(2 * (uint64_t)row) - 2;
        if (pos0 < cum + len)
            break;
        cum += len;
        ++row;
    }

    /* Position within the current row (zero‑based). */
    idx_t idx_in_row = pos0 - cum;

    /* Factor 2*row and generate its divisors (excluding 1 and 2*row). */
    uint64_t num = 2 * (uint64_t)row;
    uint64_t primes[32];
    uint64_t exps[32];
    size_t pf_cnt = 0;
    factor_u64(num, primes, exps, &pf_cnt);

    seq_t divs[128];
    size_t div_cnt = 0;
    gen_divs_rec(primes, exps, pf_cnt, 0, (seq_t)1, divs, &div_cnt, (seq_t)num);

    /* Sort divisors ascending. */
    qsort(divs, div_cnt, sizeof(seq_t), seq_cmp);

    return divs[idx_in_row];
}
