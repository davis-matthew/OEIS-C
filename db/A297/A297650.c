/*
 * A297650  Number of nX4 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 2 or 3 neighboring 1s.
 * https://oeis.org/A297650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed via dynamic programming over column patterns (no simple
 *            closed form)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: check if every '1' cell in row 'mid' has 1‑3 neighbours
   among its horizontal neighbours, and the diagonal neighbours
   from the rows 'up' and 'down'. */
static bool valid_triple[16][16][16] = {0};
static bool valid_triple_computed = false;

static inline bool is_valid_triple(int up, int mid, int down)
{
    /* For each column i (0..3) that contains a 1 in 'mid', count
       horizontal neighbours from 'mid',
       up‑left/up‑right neighbours from 'up',
       down‑left/down‑right neighbours from 'down'. */
    for (int i = 0; i < 4; ++i) {
        if (!(mid & (1 << i))) continue;
        int cnt = 0;
        if (i > 0 && (mid & (1 << (i - 1)))) ++cnt;
        if (i < 3 && (mid & (1 << (i + 1)))) ++cnt;
        if (i > 0 && (up   & (1 << (i - 1)))) ++cnt;
        if (i < 3 && (up   & (1 << (i + 1)))) ++cnt;
        if (i > 0 && (down & (1 << (i - 1)))) ++cnt;
        if (i < 3 && (down & (1 << (i + 1)))) ++cnt;
        if (cnt < 1 || cnt > 3) return false;
    }
    return true;
}

/* Compute the 16×16×16 table of valid triples (once only). */
static void compute_valid_triples(void)
{
    if (valid_triple_computed) return;
    for (int up = 0; up < 16; ++up)
        for (int mid = 0; mid < 16; ++mid)
            for (int down = 0; down < 16; ++down)
                valid_triple[up][mid][down] = is_valid_triple(up, mid, down);
    valid_triple_computed = true;
}

/* Count admissible 1×4 row patterns (used when n==1). */
static seq_t count_single_row(void)
{
    seq_t tot = 0;
    for (int pat = 0; pat < 16; ++pat) {
        bool ok = true;
        for (int i = 0; i < 4; ++i) {
            if (!(pat & (1 << i))) continue;
            int cnt = 0;
            if (i > 0 && (pat & (1 << (i - 1)))) ++cnt;
            if (i < 3 && (pat & (1 << (i + 1)))) ++cnt;
            if (cnt < 1 || cnt > 3) { ok = false; break; }
        }
        if (ok) ++tot;
    }
    return tot;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, behaviour below offset not required     */
    if (n == 1) return count_single_row();

    compute_valid_triples();

    /* DP arrays: dp[prev][curr] counts sequences ending with rows
       'prev' (row n-1) and 'curr' (row n).                 */
    seq_t dp[16][16];
    seq_t nextdp[16][16];

    /* Initialise with the first two rows.  The row above the first row
       is treated as all zeros. */
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            dp[a][b] = valid_triple[0][a][b] ? 1 : 0;

    /* Process rows 3 … n.  For each new row 'c' we check that
       row 'b' (the middle of the triple a,b,c) satisfies its
       neighbour condition. */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                nextdp[i][j] = 0;

        for (int a = 0; a < 16; ++a) {
            for (int b = 0; b < 16; ++b) {
                seq_t ways = dp[a][b];
                if (!ways) continue;
                for (int c = 0; c < 16; ++c)
                    if (valid_triple[a][b][c])
                        nextdp[b][c] += ways;
            }
        }
        /* Move nextdp into dp for the next iteration. */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = nextdp[i][j];
    }

    /* After processing n rows, dp[a][b] holds counts for
       the last two rows a (row n‑1) and b (row n).
       We must verify that the last row b is valid with
       its lower neighbour absent (treated as zeros). */
    seq_t result = 0;
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            if (valid_triple[a][b][0])
                result += dp[a][b];

    return result;
}
