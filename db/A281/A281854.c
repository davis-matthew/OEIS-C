/*
 * A281854  Irregular triangle read by rows. Row n gives the orders of the
 *          cyclic groups appearing as factors in the direct product
 *          decomposition of the abelian non-cyclic multiplicative groups of
 *          integers modulo A033949(n).
 * https://oeis.org/A281854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   factor_u64, seq_cmp, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A281854: irregular triangle rows via factorization of
 * non‑cyclic unit groups.
 * The function returns the a(n) value for any n ≥ 1 by iterating
 * rows, generating the N = A033949(row) values, factorizing N,
 * and computing the cyclic component orders. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_cmp.h"

static bool is_cyclic(uint64_t n)
{
    if (n == 1 || n == 2 || n == 4) return true;

    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);

    if (cnt == 1) {
        if (pr[0] == 2)          /* 2^k */
            return ex[0] <= 2;   /* cyclic only for k ≤ 2 */
        return true;             /* odd prime power */
    }
    if (cnt == 2) {
        if (pr[0] == 2 && ex[0] == 1) return true;
        if (pr[1] == 2 && ex[1] == 1) return true;
    }
    return false; /* otherwise non‑cyclic */
}

/* Storage for A033949 values */
static uint64_t *A033 = NULL;
static size_t A033_len = 0;
static size_t A033_cap = 0;
static uint64_t next_candidate = 2;

/* Ensure that A033[n-1] exists (1‑based index) */
static void ensure_A033(size_t n_needed)
{
    while (A033_len < n_needed) {
        if (A033_cap == 0) {
            A033_cap = 16;
            A033 = (uint64_t *)malloc(A033_cap * sizeof(uint64_t));
        } else if (A033_len >= A033_cap) {
            A033_cap <<= 1;
            A033 = (uint64_t *)realloc(A033, A033_cap * sizeof(uint64_t));
        }
        while (true) {
            if (!is_cyclic(next_candidate)) {
                A033[A033_len++] = next_candidate;
                next_candidate++;
                break;
            }
            next_candidate++;
        }
    }
}

seq_t a(idx_t n_flat)
{
    if (n_flat < 1) return 0; /* undefined, but keep safe */

    idx_t sum_len = 0;
    idx_t row = 1;

    while (true) {
        ensure_A033(row);
        uint64_t N = A033[row - 1];

        /* Factor N */
        uint64_t pr[64], ex[64];
        size_t cnt;
        factor_u64(N, pr, ex, &cnt);

        /* Build component list */
        seq_t comps[128];
        size_t comp_cnt = 0;

        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = pr[i];
            uint64_t e = ex[i];

            if (p == 2) {
                if (e == 1) { /* trivial, no component */ }
                else if (e == 2) {
                    comps[comp_cnt++] = (seq_t)2;
                } else { /* e >= 3 */
                    comps[comp_cnt++] = (seq_t)2;
                    comps[comp_cnt++] = seq_pow((seq_t)2, (idx_t)(e - 2));
                }
            } else { /* odd prime */
                if (e >= 2) {
                    comps[comp_cnt++] = seq_pow((seq_t)p, (idx_t)(e - 1));
                }
                uint64_t m = p - 1;
                if (m > 1) {
                    uint64_t pr2[64], ex2[64];
                    size_t cnt2;
                    factor_u64(m, pr2, ex2, &cnt2);
                    for (size_t j = 0; j < cnt2; ++j) {
                        comps[comp_cnt++] = seq_pow((seq_t)pr2[j], (idx_t)ex2[j]);
                    }
                }
            }
        }

        /* Sort ascending to later index from the largest */
        if (comp_cnt > 1)
            qsort(comps, comp_cnt, sizeof(seq_t), seq_cmp);

        if (sum_len + comp_cnt >= n_flat) {
            idx_t k = n_flat - sum_len - 1;              /* 0‑based position in this row */
            size_t pos = comp_cnt - 1 - k;               /* descending order uses largest first */
            return comps[pos];
        }
        sum_len += comp_cnt;
        ++row;
    }
}
