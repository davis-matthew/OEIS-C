/*
 * A345824  Numbers that are the sum of seven fourth powers in exactly two
 *          ways.
 * https://oeis.org/A345824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4073, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A345824 – Numbers that are the sum of seven fourth powers
    in exactly two distinct ways.  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* In all builds seq_t and idx_t are defined by the build system.
   This file implements an independent, memoised function.   */

static idx_t  cur_limit      = 0;          /* current pre‑computed limit */
static seq_t *terms          = NULL;       /* array of terms with 2 representations */
static size_t term_cnt       = 0;          /* number of terms stored */

/* --------------------------------------------------------------------- */
/* Rebuild the table up to a given limit.  */
static void compute_table(idx_t limit)
{
    /* Allocate DP tables for k = 0..7   */
    uint8_t *dp[8];
    for (int k = 0; k <= 7; ++k) {
        dp[k] = (uint8_t*)calloc((size_t)(limit + 1), sizeof(uint8_t));
    }
    if (!dp[0]) abort();  /* allocation failure */

    dp[0][0] = 1;          /* one way to use 0 coins to sum 0 */

    /* Gather all fourth powers <= limit */
    idx_t powv[128];       /* plenty of space for all powers <= 2e6 (≈ 37) */
    size_t pcount = 0;
    for (idx_t i = 1; ; ++i) {
        idx_t v = i * i * i * i;
        if (v > limit) break;
        powv[pcount++] = v;
    }

    /* Core DP: unlimited co‐in combinations, exactly 7 coins */
    for (size_t ci = 0; ci < pcount; ++ci) {
        idx_t v = powv[ci];
        for (int k = 1; k <= 7; ++k) {
            uint8_t *dp_k   = dp[k];
            uint8_t *dp_km1 = dp[k - 1];
            for (idx_t s = v; s <= limit; ++s) {
                uint8_t add = dp_km1[s - v];
                if (add) {
                    uint8_t val = dp_k[s] + add;
                    if (val > 3) val = 3;          /* we only need to know 2 or more */
                    dp_k[s] = val;
                }
            }
        }
    }

    /* Build the output list of sums with exactly two representations */
    seq_t *new_terms = (seq_t*)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!new_terms) abort();
    size_t new_cnt = 0;
    for (idx_t s = 0; s <= limit; ++s) {
        if (dp[7][s] == 2) {
            new_terms[new_cnt++] = (seq_t)s;
        }
    }

    /* Free old dp tables */
    for (int k = 0; k <= 7; ++k) {
        free(dp[k]);
    }

    /* Replace old list */
    free(terms);
    terms     = new_terms;
    term_cnt  = new_cnt;
    cur_limit = limit;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; this case is undefined in OEIS */

    /* Initialise or enlarge the table lazily */
    if (cur_limit == 0) {
        compute_table(2000000);   /* initial guess */
    }
    while (term_cnt < n) {
        compute_table(cur_limit * 2);
    }
    return terms[n - 1];
}
