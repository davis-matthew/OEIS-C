/*
 * A283096  Number of nX4 0..1 arrays with no 1 equal to more than one of its
 *          horizontal and vertical neighbors, with the exception of exactly
 *          two elements.
 * https://oeis.org/A283096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 0..1 arrays where every 1 has at most one *
 *            neighboring 1 (up, down, left, or right), except exactly two
 *            cells may * violate this rule. a(1) = 1, a(2) = 28, … (OEIS
 *            A283096)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   violations, violations_last
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/violations.h"
#include "../utils/violations_last.h"

/* Pre‑computed tables of violations. */
static struct {
    int ready;
    seq_t v[16][16][16];   /* v[p][q][r]            */
    seq_t lv[16][16];      /* lv[p][q] (final row) */
} _tab = { 0 };

/* Initialise the tables once. */
static void init_tables(void)
{
    if (_tab.ready) return;
    for (uint16_t p = 0; p < 16; ++p) {
        for (uint16_t q = 0; q < 16; ++q) {
            _tab.lv[p][q] = (seq_t)violations_last(p, q);
            for (uint16_t r = 0; r < 16; ++r)
                _tab.v[p][q][r] = (seq_t)violations(p, q, r);
        }
    }
    _tab.ready = 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_tables();
    if (n == 0) return 0;  /* not used, but safe */
    /* dp[p][q][e] : count after processing some rows,
     * where the last two rows are p (up) and q (current),
     * and e is the number of violations so far (0..2). */
    seq_t dp[16][16][3]   = { 0 };
    seq_t ndp[16][16][3]  = { 0 };

    dp[0][0][0] = 1;  /* start state */

    for (idx_t step = 0; step < n; ++step) {
        /* zero new dp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                for (int e = 0; e < 3; ++e)
                    ndp[i][j][e] = 0;

        /* transition */
        for (int p = 0; p < 16; ++p) {
            for (int q = 0; q < 16; ++q) {
                for (int e = 0; e < 3; ++e) {
                    seq_t ways = dp[p][q][e];
                    if (!ways) continue;
                    for (int r = 0; r < 16; ++r) {
                        int v = (int)_tab.v[p][q][r];
                        int ne = e + v;
                        if (ne <= 2) ndp[q][r][ne] += ways;
                    }
                }
            }
        }

        /* swap dp and ndp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                for (int e = 0; e < 3; ++e)
                    dp[i][j][e] = ndp[i][j][e];
    }

    /* After all rows, finalize the last row. */
    seq_t result = 0;
    for (int p = 0; p < 16; ++p) {
        for (int r = 0; r < 16; ++r) {
            int lv = (int)_tab.lv[p][r];
            int need = 2 - lv;
            if (need < 0 || need > 2) continue;
            result += dp[p][r][need];
        }
    }
    return result;
}
