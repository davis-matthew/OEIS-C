/*
 * A275224  Number of nX4 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,1) or (-1,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed via a dynamic programming recurrence: the
 *            number of n×4 arrays over {0,1,2} with the forbidden offsets and
 *            incremental value rule. The DP iterates over all 3⁴ = 81 row
 *            patterns, maintaining the previous two rows and the maximum
 *            value seen so far (0,1,2). The implementation is O(n...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define PATTS   81   /* 3⁴ patterns */
#define P2MAX   82   /* previous‑row patterns + sentinel */
#define MAXM    3    /* possible current maximum 0,1,2 */
#define SENTINEL (P2MAX-1) /* sentinel index for “no previous row” */

/* Global tables filled once by precompute() */
static unsigned char patt[PATTS][4];
static unsigned char allowed[PATTS][MAXM];
static unsigned char newmax[PATTS][MAXM];
static unsigned char trans_next[PATTS][P2MAX][MAXM][PATTS];
static unsigned char trans_newmax[PATTS][P2MAX][MAXM][PATTS];
static unsigned char trans_cnt[PATTS][P2MAX][MAXM];

static int initialized = 0;

static void precompute(void)
{
    if (initialized) return;
    initialized = 1;

    /* Enumerate all 4‑cell patterns (base‑3 representation) */
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c)
                for (int d = 0; d < 3; ++d) {
                    int idx = a * 27 + b * 9 + c * 3 + d;
                    patt[idx][0] = (unsigned char)a;
                    patt[idx][1] = (unsigned char)b;
                    patt[idx][2] = (unsigned char)c;
                    patt[idx][3] = (unsigned char)d;
                }

    /* Compute allowedness and new maximum for each pattern at each start maximum */
    for (int p = 0; p < PATTS; ++p)
        for (int sm = 0; sm < MAXM; ++sm) {
            int cur = sm;
            int ok = 1;
            for (int j = 0; j < 4; ++j) {
                int c = patt[p][j];
                if (c > cur + 1) { ok = 0; break; }
                if (c == cur + 1) cur = c;
            }
            allowed[p][sm] = (unsigned char)ok;
            newmax[p][sm]   = (unsigned char)cur;
        }

    /* Precompute transitions between rows */
    for (int prev = 0; prev < PATTS; ++prev)
        for (int prev2 = 0; prev2 < P2MAX; ++prev2)
            for (int sm = 0; sm < MAXM; ++sm) {
                unsigned char *next_arr = trans_next[prev][prev2][sm];
                unsigned char *nm_arr   = trans_newmax[prev][prev2][sm];
                unsigned char cnt = 0;
                for (int cur = 0; cur < PATTS; ++cur) {
                    if (!allowed[cur][sm]) continue;          /* row independently valid */
                    int ok = 1;
                    for (int j = 0; j < 4 && ok; ++j) {
                        int c = patt[cur][j];
                        /* same column in previous row */
                        if (c == patt[prev][j]) ok = 0;
                        /* diagonally two rows above */
                        if (ok && prev2 != SENTINEL) {
                            if (j > 0 && c == patt[prev2][j-1]) ok = 0;
                            if (ok && j < 3 && c == patt[prev2][j+1]) ok = 0;
                        }
                    }
                    if (!ok) continue;
                    next_arr[cnt] = (unsigned char)cur;
                    nm_arr[cnt]   = newmax[cur][sm];
                    ++cnt;
                }
                trans_cnt[prev][prev2][sm] = cnt;
            }
}

/* DP arrays allocated on first call */
static seq_t *dp_cur = NULL;
static seq_t *dp_nxt = NULL;
static size_t dp_size = 0;

seq_t a(idx_t n)
{
    precompute();

    /* size of DP state space */
    size_t sz = (size_t)PATTS * P2MAX * MAXM;
    if (dp_cur == NULL || dp_size < sz) {
        free(dp_cur);
        free(dp_nxt);
        dp_cur = (seq_t*)calloc(sz, sizeof(seq_t));
        dp_nxt = (seq_t*)calloc(sz, sizeof(seq_t));
        dp_size = sz;
    }
    /* reset current DP */
    memset(dp_cur, 0, sz * sizeof(seq_t));

    /* ---- initialise first row (row 1) ---- */
    for (int r = 0; r < PATTS; ++r) {
        if (patt[r][0] != 0) continue;          /* first cell must be 0 */
        if (!allowed[r][0]) continue;            /* pattern must be allowed from sm=0 */
        int m = newmax[r][0];
        size_t idx = ((size_t)r * P2MAX + SENTINEL) * MAXM + (size_t)m;
        dp_cur[idx] = (seq_t)1;
    }

    /* ---- iterate subsequent rows ---- */
    for (idx_t row = 2; row <= n; ++row) {
        memset(dp_nxt, 0, sz * sizeof(seq_t));
        for (int p1 = 0; p1 < PATTS; ++p1)
            for (int p2 = 0; p2 < P2MAX; ++p2)
                for (int m = 0; m < MAXM; ++m) {
                    size_t pos = ((size_t)p1 * P2MAX + p2) * MAXM + (size_t)m;
                    seq_t val = dp_cur[pos];
                    if (val == (seq_t)0) continue;
                    unsigned char *next_arr = trans_next[p1][p2][m];
                    unsigned char *nm_arr   = trans_newmax[p1][p2][m];
                    unsigned char cnt = trans_cnt[p1][p2][m];
                    for (unsigned char t = 0; t < cnt; ++t) {
                        int r = next_arr[t];
                        int nm = nm_arr[t];
                        size_t npos = ((size_t)r * P2MAX + p1) * MAXM + (size_t)nm;
                        dp_nxt[npos] += val;
                    }
                }
        /* swap dp_cur and dp_nxt */
        seq_t *tmp = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;
    }

    /* ---- sum all end states ---- */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < sz; ++i) {
        total += dp_cur[i];
    }
    return total;
}
