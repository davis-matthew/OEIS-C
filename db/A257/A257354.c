/*
 * A257354  Number of (n+2)X(1+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum equal to the antidiagonal sum or central row sum less than the
 *          central column sum.
 * https://oeis.org/A257354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×3 binary arrays with no * 3×3 subblock
 *            having diagonal sum equal to antidiagonal sum, * and with
 *            central row sum not less than the central column sum. * The
 *            sequence is computed by dynamic programming over triples of *
 *            consecutive rows
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static int _a257354_initialized = 0;
static int _a257354_nextcnt[64];
static int _a257354_nextlist[64][8];

/* Precomputes transitions between pairs of rows:
   from pair (p,q) to next row r if the 3×3 block (p,q,r) satisfies
   the required constraints. */
static void _a257354_init(void) {
    if (_a257354_initialized) return;
    _a257354_initialized = 1;

    int bit[8][3];
    int rowSum[8];
    for (int p = 0; p < 8; ++p) {
        for (int c = 0; c < 3; ++c)
            bit[p][c] = (p >> c) & 1;
        rowSum[p] = bit[p][0] + bit[p][1] + bit[p][2];
    }

    for (int a = 0; a < 8; ++a) {
        for (int b = 0; b < 8; ++b) {
            int idx = a * 8 + b;
            _a257354_nextcnt[idx] = 0;
            for (int r = 0; r < 8; ++r) {
                int diag = bit[a][0] + bit[b][1] + bit[r][2];
                int anti = bit[a][2] + bit[b][1] + bit[r][0];
                if (diag == anti) continue;               /* disallow equal sums */

                int row2 = rowSum[b];
                int col2 = bit[a][1] + bit[b][1] + bit[r][1];
                if (row2 < col2) continue;                /* disallow smaller row sum */

                _a257354_nextlist[idx][_a257354_nextcnt[idx]++] = r;
            }
        }
    }
}

/* DP over length L = n+2 rows. */
seq_t a(idx_t n) {
    _a257354_init();
    if (n == 0) return 0;                /* offset is 1; for n<1 result is undefined */

    idx_t L = n + 2;                     /* number of rows */
    if (L < 3) return 0;                 /* no 3×3 blocks when L<3 */

    seq_t curr[64];
    seq_t next[64];
    for (int i = 0; i < 64; ++i) curr[i] = 1;   /* all pairs of two rows */

    for (idx_t step = 3; step <= L; ++step) {
        for (int i = 0; i < 64; ++i) next[i] = 0;
        for (int p = 0; p < 64; ++p) {
            seq_t v = curr[p];
            if (v == 0) continue;
            int cnt = _a257354_nextcnt[p];
            int base = p & 7;                /* lower 3 bits are previous row */
            for (int k = 0; k < cnt; ++k) {
                int r = _a257354_nextlist[p][k];
                int nid = base * 8 + r;      /* new pair (prev, r) */
                next[nid] += v;
            }
        }
        for (int i = 0; i < 64; ++i) curr[i] = next[i];
    }

    seq_t result = 0;
    for (int i = 0; i < 64; ++i) result += curr[i];
    return result;
}
