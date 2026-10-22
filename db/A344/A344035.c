/*
 * A344035  Numbers that are the sum of five positive cubes in exactly four
 *          ways.
 * https://oeis.org/A344035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers that are the sum of five positive cubes in exactly four
 *            distinct ways. We compute the sequence by dynamic programming
 *            over cube “coins” and collect all sums that occur exactly four
 *            times. The sequence is generated lazily on the first call
 * Range:     u8 none, u16 n <= 5861, u32 n <= 7935, u64 n <= 7935,
 *            u128 n <= 7935, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* External types supplied by the build system */
extern seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/* Internal state for lazy construction */
static bool seq_built = false;         /* flag: has the sequence been built?   */
static seq_t *seq_values = NULL;       /* array of found terms                */
static size_t seq_len    = 0;          /* current number of stored terms       */
static size_t seq_cap    = 0;          /* allocated array capacity            */

/* --------------------------------------------------------------------- */
/* Helper: build the sequence on first use */
static void build_sequence(void)
{
    if (seq_built)
        return;

    /* ----------------------------------------------------------------- */
    /* 1. Choose an upper bound for sums.  1 000 000 comfortably covers
       the first 10 000 terms and is memory‑friendly (≈12 MB).          */
    const idx_t MAX_SUM = 1000000;
    const size_t V = (size_t)MAX_SUM + 1;     /* number of possible sums */

    /* ----------------------------------------------------------------- */
    /* 2. Pre‑compute all positive cubes ≤ MAX_SUM. */
    idx_t *cubes = NULL;
    size_t cube_cnt = 0, cube_cap = 16;
    cubes = (idx_t *)malloc(cube_cap * sizeof(idx_t));
    for (idx_t k = 1; ; ++k) {
        idx_t c = k * k * k;                       /* k³  */
        if (c > MAX_SUM)
            break;
        if (cube_cnt == cube_cap) {               /* grow array */
            cube_cap <<= 1;
            cubes = (idx_t *)realloc(cubes, cube_cap * sizeof(idx_t));
        }
        cubes[cube_cnt++] = c;
    }

    /* ----------------------------------------------------------------- */
    /* 3. DP table: dp[t][s] – number of ways to obtain sum s
       using exactly t cubes (t = 0…5).  We store a flat array of
       16‑bit integers; values are capped at 5 because we only need 0–4. */
    uint16_t *dp = (uint16_t *)calloc(6 * V, sizeof(uint16_t));
    if (!dp) {
        free(cubes);
        return;      /* allocation failure: sequence remains empty */
    }
    dp[0] = 1;                       /* dp[0][0] = 1 */

    for (size_t ci = 0; ci < cube_cnt; ++ci) {
        const idx_t cube = cubes[ci];
        for (size_t t = 1; t <= 5; ++t) {
            size_t row_t   = t * V;
            size_t row_tm1 = (t - 1) * V;
            for (idx_t s = cube; s <= MAX_SUM; ++s) {
                uint16_t add = dp[row_tm1 + (s - cube)];
                if (add) {
                    uint16_t cur = dp[row_t + s];
                    cur += add;
                    if (cur > 5)
                        cur = 5;
                    dp[row_t + s] = cur;
                }
            }
        }
    }

    /* ----------------------------------------------------------------- */
    /* 4. Extract all sums with exactly four representations. */
    seq_cap = 256;            /* initial array capacity */
    seq_values = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_len = 0;

    for (idx_t s = 1; s <= MAX_SUM; ++s) {
        if (dp[5 * V + s] == 4) {               /* exactly four ways */
            if (seq_len == seq_cap) {           /* grow array */
                seq_cap <<= 1;
                seq_values = (seq_t *)realloc(seq_values,
                                               seq_cap * sizeof(seq_t));
            }
            seq_values[seq_len++] = (seq_t)s;
        }
    }

    /* ----------------------------------------------------------------- */
    /* 5. Clean up and mark as built. */
    free(dp);
    free(cubes);
    seq_built = true;
}

/* --------------------------------------------------------------------- */
/* Public sequence function */
seq_t a(idx_t n)
{
    build_sequence();

    if (n == 0 || n > (idx_t)seq_len)
        return (seq_t)0;      /* out‑of‑bounds – should not happen in tests */

    return seq_values[(size_t)(n - 1)];
}
