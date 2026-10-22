/*
 * A339486  Number of possible destination squares after n knight's moves on
 *          an 8x8 chessboard beginning on a corner square.
 * https://oeis.org/A339486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simulates knight moves on an 8×8 chessboard starting from a corner.
   Returns the number of distinct squares that can be reached after n
   moves.  After 5 moves the set saturates, so a(n)=32 for all n≥5. */
#include <stdint.h>

/* Count 1-bits in a 64‑bit word */
static unsigned popcount64(uint64_t x)
{
    unsigned c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* n = 0: only the starting corner is reachable */
    if (n == 0)
        return (seq_t)1;

    /* bit mask of squares reachable after the current number of moves */
    uint64_t mask = 1ULL;            /* corner (0,0) is bit 0 */

    /* knight move offsets */
    const int dr[8] = {  2,  2, -2, -2,  1,  1, -1, -1 };
    const int dc[8] = {  1, -1,  1, -1,  2, -2,  2, -2 };

    for (idx_t step = 1; step <= n; ++step) {
        uint64_t next = 0;

        /* expand to all squares reachable in one move from current mask */
        for (int s = 0; s < 64; ++s) {
            if (mask & (1ULL << s)) {
                int r = s / 8;
                int c = s % 8;
                for (int mv = 0; mv < 8; ++mv) {
                    int nr = r + dr[mv];
                    int nc = c + dc[mv];
                    if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8)
                        next |= 1ULL << (nr * 8 + nc);
                }
            }
        }

        /* If we have reached the requested step, return the count. */
        if (step == n)
            return (seq_t)popcount64(next);

        /* If no new squares appear, all further steps give the same count. */
        if (next == mask)
            return (seq_t)popcount64(next);

        /* Prepare for the next iteration */
        mask = next;
    }

    /* Should never reach here */
    return 0;
}
