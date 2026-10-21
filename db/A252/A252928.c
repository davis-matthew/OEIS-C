/*
 * A252928  Number of nX6 nonnegative integer arrays with upper left 0 and
 *          lower right n+6-6 and value increasing by 0 or 1 with every step
 *          right or down.
 * https://oeis.org/A252928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of n×6 nonnegative integer arrays with * upper
 *            left 0, lower right n, and value increasing by 0 or 1 * with
 *            every step right or down. * Computation implemented with a
 *            dynamic program over row patterns
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 30, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>   /* for memset */

static int base[32][7];
static int popcnt[32];
static int translist[32][2][32];
static int transcnt[32][2];

static void init_tables(void)
{
    /* Precompute prefix sums of increments for every 5‑bit pattern */
    for (int mask = 0; mask < 32; ++mask) {
        base[mask][1] = 0;
        int cnt = 0;
        for (int j = 2; j <= 6; ++j) {
            int bitpos = j - 2;          /* bits correspond to columns 2..6 */
            if (mask & (1 << bitpos))
                ++cnt;
            base[mask][j] = cnt;
        }
        popcnt[mask] = cnt;
    }
    /* Precompute feasible transitions for each (prev_pattern, Δa) */
    for (int p = 0; p < 32; ++p) {
        for (int da = 0; da <= 1; ++da) {
            transcnt[p][da] = 0;
            for (int q = 0; q < 32; ++q) {
                int ok = 1;
                for (int j = 1; j <= 6; ++j) {
                    int diff = da + base[q][j] - base[p][j];
                    if (diff < 0 || diff > 1) { /* must be 0 or 1 */
                        ok = 0;
                        break;
                    }
                }
                if (ok) {
                    translist[p][da][transcnt[p][da]++] = q;
                }
            }
        }
    }
}

/* Count of values in the sequence for given index n (n >= 1) */
seq_t a(idx_t n)
{
    /* initialize transition tables once */
    static int tables_built = 0;
    if (!tables_built) {
        init_tables();
        tables_built = 1;
    }

    /* array size: 32 patterns × (n+1) possible first‑column values */
    const size_t P = 32;
    const size_t dim = P * (n + 1);
    /* allocate DP arrays */
    seq_t *dp_cur  = (seq_t*)malloc(sizeof(seq_t) * dim);
    seq_t *dp_next = (seq_t*)malloc(sizeof(seq_t) * dim);
    if (!dp_cur || !dp_next) {
        /* Allocation failed – return 0 as a safe fallback. */
        if (dp_cur) free(dp_cur);
        if (dp_next) free(dp_next);
        seq_t zero = 0;
        return zero;
    }
    /* initialise to zero */
    memset(dp_cur, 0, sizeof(seq_t) * dim);
    memset(dp_next, 0, sizeof(seq_t) * dim);

    /* row 1: first column value is 0, any pattern is allowed */
    for (int p = 0; p < 32; ++p) {
        dp_cur[p * (n + 1) + 0] = 1;
    }

    /* process subsequent rows */
    for (idx_t r = 2; r <= n; ++r) {
        /* zero dp_next for this row */
        memset(dp_next, 0, sizeof(seq_t) * dim);

        for (int p = 0; p < 32; ++p) {
            size_t idx_base = (size_t)p * (n + 1);
            for (idx_t a1 = 0; a1 < r; ++a1) {
                seq_t val = dp_cur[idx_base + a1];
                if (val == 0)
                    continue;
                for (int da = 0; da <= 1; ++da) {
                    idx_t new_a1 = a1 + da;
                    if (new_a1 > r)  /* should not happen but keep safe */
                        continue;
                    int *qlist = translist[p][da];
                    int qcnt   = transcnt[p][da];
                    for (int t = 0; t < qcnt; ++t) {
                        int q = qlist[t];
                        dp_next[(size_t)q * (n + 1) + new_a1] += val;
                    }
                }
            }
        }
        /* swap dp_cur and dp_next for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* compute final answer: a(n) = sum over patterns where
     * first_column + popcount(pattern) == n */
    seq_t ans = 0;
    for (int p = 0; p < 32; ++p) {
        int need_a1 = (int)n - popcnt[p];
        if (need_a1 < 0 || need_a1 > n)
            continue;
        ans += dp_cur[p * (n + 1) + need_a1];
    }

    free(dp_cur);
    free(dp_next);
    return ans;
}
