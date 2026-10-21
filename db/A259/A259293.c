/*
 * A259293  Number of (n+1)X(4+1) 0..1 arrays with each 2X2 subblock having
 *          clockwise pattern 0001 0101 or 0111.
 * https://oeis.org/A259293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A259293: Number of (n+1)×5 0/1 arrays whose every 2×2 subblock
   consists of one of the 10 patterns obtained by rotating
   the strings 0001, 0101, or 0111 clockwise.
   The 10 allowed 4‑bit values are:
        0001 (1), 0010 (2), 0100 (4), 0101 (5),
        0111 (7), 1000 (8), 1010 (10), 1011 (11),
        1101 (13), 1110 (14).
   The function builds the 32×32 adjacency table once and
   uses dynamic programming over rows to count the arrays. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* adjacency matrix for transitions between rows (5 bits) */
    static bool adj[32][32];
    static bool ready = false;

    if (!ready) {
        /* allowed 4‑bit values (rotations of 0001,0101,0111) */
        bool allowed[16] = {0};
        const uint8_t vals[] = {1, 2, 4, 5, 7, 8, 10, 11, 13, 14};
        for (unsigned i = 0; i < sizeof(vals); ++i)
            allowed[vals[i]] = true;

        /* build adjacency */
        for (unsigned r1 = 0; r1 < 32; ++r1) {
            for (unsigned r2 = 0; r2 < 32; ++r2) {
                bool ok = true;
                for (unsigned j = 0; j < 4; ++j) {
                    unsigned a = (r1 >> j) & 1;          /* top‑left  */
                    unsigned b = (r1 >> (j + 1)) & 1;    /* top‑right */
                    unsigned c = (r2 >> j) & 1;          /* bot‑left  */
                    unsigned d = (r2 >> (j + 1)) & 1;    /* bot‑right */
                    unsigned p = (a << 3) | (b << 2) |
                                 (d << 1) | (c);          /* a b d c */
                    if (!allowed[p]) {
                        ok = false;
                        break;
                    }
                }
                adj[r1][r2] = ok;
            }
        }
        ready = true;
    }

    /* DP over rows: dp[i] = number of arrays ending with row i */
    seq_t dp[32];
    seq_t ndp[32];
    for (unsigned i = 0; i < 32; ++i)
        dp[i] = (seq_t)1;     /* first row can be arbitrary */

    for (idx_t step = 0; step < n; ++step) {   /* n transitions -> n+1 rows */
        for (unsigned i = 0; i < 32; ++i)
            ndp[i] = 0;
        for (unsigned r1 = 0; r1 < 32; ++r1) {
            seq_t val = dp[r1];
            if (!val) continue;
            for (unsigned r2 = 0; r2 < 32; ++r2) {
                if (adj[r1][r2])
                    ndp[r2] += val;
            }
        }
        for (unsigned i = 0; i < 32; ++i)
            dp[i] = ndp[i];
    }

    /* sum all ending rows */
    seq_t result = 0;
    for (unsigned i = 0; i < 32; ++i)
        result += dp[i];
    return result;
}
