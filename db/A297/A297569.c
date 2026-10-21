/*
 * A297569  Number of nonisomorphic proper colorings of partition star graph
 *          using five colors.
 * https://oeis.org/A297569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = k * ∏_{p^v∈λ} C((k-1)^p + v - 1, v) with k = 5 Ordering:
 *            partitions enumerated from sum = 0 upward, and for each sum the
 *            partitions are listed in lexicographic order of their parts in
 *            non‐increasing sequence
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required utility header providing binomial() */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

static size_t max_size = 0;          /* current maximum sum computed */
static seq_t *cum = NULL;            /* cumulative counts up to each sum */
static seq_t **dp2 = NULL;           /* dp2[rem][max_part] = #partitions of rem
                                      with parts ≤ max_part */

/* compute dp2 and cum up to newS (inclusive), recomputing from scratch */
static void prepare_up_to(size_t newS)
{
    if (newS <= max_size) return;

    /* allocate cumulative array */
    seq_t *newcum = NULL;
    if (newS > 0) {
        newcum = (seq_t *)realloc(cum, (newS + 1) * sizeof(seq_t));
        if (!newcum) abort();
    }
    cum = newcum;

    /* allocate dp2 rows */
    seq_t **newdp = NULL;
    if (newS > 0) {
        newdp = (seq_t **)realloc(dp2, (newS + 1) * sizeof(seq_t *));
        if (!newdp) abort();
    }
    dp2 = newdp;

    /* allocate each row and fill */
    for (size_t i = 0; i <= newS; ++i) {
        seq_t *row = NULL;
        if (newS > 0) {
            row = (seq_t *)realloc(dp2[i], (newS + 1) * sizeof(seq_t));
            if (!row) abort();
        }
        dp2[i] = row;
    }

    /* DP: dp2[rem][maxp] */
    for (size_t rem = 0; rem <= newS; ++rem) {
        for (size_t maxp = 0; maxp <= newS; ++maxp) {
            if (rem == 0) {
                dp2[rem][maxp] = (seq_t)1;
            } else if (maxp == 0) {
                dp2[rem][maxp] = (seq_t)0;
            } else {
                if (rem >= maxp) {
                    dp2[rem][maxp] = dp2[rem][maxp - 1] + dp2[rem - maxp][maxp];
                } else {
                    dp2[rem][maxp] = dp2[rem][maxp - 1];
                }
            }
        }
    }

    /* cumulative sums: cum[s] = Σ_{t=0}^s p(t) */
    for (size_t s = 0; s <= newS; ++s) {
        if (s == 0) {
            cum[s] = dp2[0][0];          /* p(0) = 1 */
        } else {
            cum[s] = cum[s - 1] + dp2[s][s];
        }
    }

    max_size = newS;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* initialise tables up to a safe bound (≈30 covers n ≤ 10 000) */
    static bool init = false;
    if (!init) {
        prepare_up_to(30);
        init = true;
    }

    /* find smallest s with cumulative sum > n */
    size_t s = 0;
    while (s <= max_size && (idx_t)cum[s] <= n) ++s;

    /* if n is beyond current tables enlarge */
    if (s > max_size) {
        size_t newS = max_size * 2 + 10;   /* increase redundantly */
        if (newS < s) newS = s;
        prepare_up_to(newS);
        while (s <= max_size && (idx_t)cum[s] <= n) ++s;
    }

    /* offset of n within partitions of sum s */
    seq_t offset = (seq_t)n;
    if (s > 0) offset -= cum[s - 1];

    /* generate the offset‑th partition of sum s (lexicographic order) */
    size_t parts[64];          /* s ≤ 30 so 64 slots are enough */
    size_t parts_len = 0;
    size_t rem = s;
    while (rem > 0) {
        for (size_t a = 1; a <= rem; ++a) {
            seq_t cnt = dp2[rem - a][a];
            if (offset >= cnt) {
                offset -= cnt;
            } else {
                parts[parts_len++] = a;
                rem -= a;
                break;
            }
        }
    }

    /* evaluate the OCP formula */
    seq_t val = (seq_t)5;      /* k = 5 */

    for (size_t i = 0; i < parts_len; ) {
        size_t p = parts[i];
        size_t j = i;
        while (j < parts_len && parts[j] == p) ++j;
        size_t v = j - i;                       /* multiplicity of part p */

        idx_t base = (idx_t)1ULL << (2 * p);    /* (k-1)^p = 4^p */
        idx_t top  = base + (idx_t)v - 1ULL;    /* top of binomial */

        seq_t bin = binomial(top, (idx_t)v);
        val = val * bin;
        i = j;
    }

    return val;
}
