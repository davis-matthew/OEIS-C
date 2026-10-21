/*
 * A251295  Number of (n+1)X(3+1) 0..3 arrays with every 2X2 subblock summing
 *          to 3 6 or 9.
 * https://oeis.org/A251295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to build an (n+1)×4 array of 0..3 * such
 *            that every 2×2 subblock sums to 3, 6 or 9. * Computed by dynamic
 *            programming over rows: each row is a 4‑digit * base‑4 number
 *            (0‑255). Two consecutive rows are compatible iff * for each of
 *            the three 2×2 blocks the sum of the four entr...
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds the adjacency lists once, then
 * performs an O(n·E) DP where E is the number of compatible row
 * pairs.  This safely works for any seq_t (including arbitrary‑precision
 * integer types) because only additions are performed. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static bool _initialized = false;
static unsigned char _neigh[256][256];
static unsigned char _neigh_cnt[256];
static unsigned char _digit[256][4];

/* Build adjacency lists for 256 possible rows. */
static void _build(void)
{
    /* Pre‑compute digits of every row. */
    for (int r = 0; r < 256; ++r) {
        _digit[r][0] = (r >> 0) & 3;
        _digit[r][1] = (r >> 2) & 3;
        _digit[r][2] = (r >> 4) & 3;
        _digit[r][3] = (r >> 6) & 3;
    }
    for (int a = 0; a < 256; ++a)
        _neigh_cnt[a] = 0;
    for (int a = 0; a < 256; ++a) {
        for (int b = 0; b < 256; ++b) {
            bool ok = true;
            for (int j = 0; j < 3 && ok; ++j) {
                int s = _digit[a][j] + _digit[a][j + 1] +
                        _digit[b][j] + _digit[b][j + 1];
                if (s != 3 && s != 6 && s != 9)
                    ok = false;
            }
            if (ok) {
                _neigh[a][_neigh_cnt[a]++] = (unsigned char)b;
            }
        }
    }
    _initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!_initialized)
        _build();

    static seq_t curr[256];
    static seq_t next[256];

    /* Start with a single row. */
    for (int i = 0; i < 256; ++i)
        curr[i] = (seq_t)1;

    /* Perform n transitions (producing n+1 rows). */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 256; ++i)
            next[i] = (seq_t)0;

        for (int i = 0; i < 256; ++i) {
            seq_t ci = curr[i];
            if (ci == (seq_t)0)
                continue;

            unsigned char *nbr = _neigh[i];
            unsigned char cnt = _neigh_cnt[i];

            for (int k = 0; k < cnt; ++k) {
                unsigned char j = nbr[k];
                next[j] = next[j] + ci;
            }
        }

        /* Prepare for next iteration. */
        for (int i = 0; i < 256; ++i)
            curr[i] = next[i];
    }

    /* Sum all final counts. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 256; ++i)
        total = total + curr[i];

    return total;
}
