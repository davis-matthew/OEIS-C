/*
 * A251152  Number of (n+1)X(2+1) 0..2 arrays with no 2X2 subblock having the
 *          minimum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over {0,1,2} in which * no 2×2
 *            subblock has min(diagonal elements) > |(off‑diagonal)
 *            difference|. * Computation performed by a 27‑state transfer
 *            matrix: * a(1)=470, a(2)=8407, a(3)=148833, … * For any n≥1 the
 *            amount is obtained by iterating a 27×27 adjacency matrix...
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static bool _table_initialized = false;
static bool _allowed[27][27];

/* Precompute the 27×27 transition matrix according to the sub‑block rule. */
static void _init_allowed(void) {
    if (_table_initialized) return;
    for (int p = 0; p < 27; ++p) {
        int prev[3];
        prev[0] = p % 3;
        prev[1] = (p / 3) % 3;
        prev[2] = (p / 9) % 3;
        for (int q = 0; q < 27; ++q) {
            int cur[3];
            cur[0] = q % 3;
            cur[1] = (q / 3) % 3;
            cur[2] = (q / 9) % 3;

            /* j = 0 subblock */
            int min0 = prev[0] < cur[1] ? prev[0] : cur[1];
            int diff0 = prev[1] - cur[0];
            if (diff0 < 0) diff0 = -diff0;
            bool ok0 = (min0 <= diff0);

            /* j = 1 subblock */
            int min1 = prev[1] < cur[2] ? prev[1] : cur[2];
            int diff1 = prev[2] - cur[1];
            if (diff1 < 0) diff1 = -diff1;
            bool ok1 = (min1 <= diff1);

            _allowed[p][q] = ok0 && ok1;
        }
    }
    _table_initialized = true;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;       /* sequence offset is 1 */

    _init_allowed();

    /* DP arrays: current and next row counts */
    seq_t cur[27];
    seq_t nxt[27];
    for (int s = 0; s < 27; ++s) cur[s] = 1;    /* first row can be any state */

    /* Apply transitions for n steps (n+1 rows → n transitions) */
    for (idx_t step = 0; step < n; ++step) {
        for (int s = 0; s < 27; ++s) nxt[s] = 0;
        for (int p = 0; p < 27; ++p) {
            if (cur[p] == 0) continue;          /* skip zero rows */
            for (int q = 0; q < 27; ++q) {
                if (_allowed[p][q])
                    nxt[q] = nxt[q] + cur[p];
            }
        }
        for (int s = 0; s < 27; ++s) cur[s] = nxt[s];
    }

    /* Sum over last row states */
    seq_t total = 0;
    for (int s = 0; s < 27; ++s) total = total + cur[s];
    return total;
}
