/*
 * A251224  Number of (n+1)X(4+1) 0..1 arrays with no 2X2 subblock having the
 *          minimum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 binary arrays with no 2×2 subblock
 *            having both diagonal entries 1 and equal anti‑diagonal entries
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is computed by dynamic programming: for each possible
   5‑bit row a bitmask of reachable next rows is pre‑computed once.
   For a given n we perform n transitions starting from all 32
   possible first rows. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Transition table: transition[src] is a 32‑bit mask of
       destination rows that can follow src.  The table is built
       only once. */
    static bool initialized = false;
    static uint32_t transition[32];

    if (!initialized) {
        for (uint32_t src = 0; src < 32; ++src) {
            uint32_t mask = 0;
            for (uint32_t dest = 0; dest < 32; ++dest) {
                bool ok = true;
                /* Check the 4 possible 2×2 subblocks. */
                for (uint32_t j = 0; j < 4; ++j) {
                    unsigned b00 = (src >> j) & 1u;           /* top‑left   */
                    unsigned b11 = (dest >> (j + 1)) & 1u;     /* bottom‑right */
                    unsigned b01 = (src >> (j + 1)) & 1u;      /* top‑right  */
                    unsigned b10 = (dest >> j) & 1u;           /* bottom‑left */
                    if (b00 && b11 && (b01 == b10)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) mask |= (1u << dest);
            }
            transition[src] = mask;
        }
        initialized = true;
    }

    /* DP arrays: cur[row] holds the number of ways to reach
       the current row state.  nxt[] will accumulate the next
       layer. */
    seq_t cur[32];
    seq_t nxt[32];

    /* Start with a single row – every 5‑bit pattern is allowed. */
    for (int i = 0; i < 32; ++i)
        cur[i] = (seq_t)1;

    /* Perform n transitions (leading to n+1 rows in total). */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < 32; ++i)
            nxt[i] = (seq_t)0;
        for (int src = 0; src < 32; ++src) {
            seq_t val = cur[src];
            if (val == (seq_t)0) continue;
            uint32_t mask = transition[src];
            for (int dest = 0; dest < 32; ++dest) {
                if (mask & (1u << dest))
                    nxt[dest] += val;
            }
        }
        for (int i = 0; i < 32; ++i)
            cur[i] = nxt[i];
    }

    /* Sum all ending states to obtain the full count. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 32; ++i)
        total += cur[i];

    return total;
}
