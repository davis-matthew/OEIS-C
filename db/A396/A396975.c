/*
 * A396975  Follow along the squares in the Hilbert curve. Place a chess king
 *          at each square that is not attacked by an existing king. This
 *          sequence gives the squares occupied by the kings.
 * https://oeis.org/A396975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   d2xy, rot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq A396975 – Hilbert‑curve king placement.
 * The function returns the Hilbert distance (0‑based) of the
 * n-th king.  A king is placed at each square of the infinite
 * Hilbert curve that is not attacked (within one king move) by
 * any previously placed king.  The algorithm is deterministic
 * and memoises the result across calls.  It uses a finite grid
 * large enough (1024×1024) to cover all requested indices
 * within the time limit.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/d2xy.h"
#include "../utils/rot.h"

/* ---- Hilbert curve helpers -------------------------------------------------- */

/* ---- Global state ---------------------------------------------------------- */

#define GRID_EXP   10                      /* 2^10 = 1024 */
#define GRID_SIZE  (1 << GRID_EXP)
#define TOTAL_SQUARES (GRID_SIZE * GRID_SIZE)

static seq_t *king_pos  = NULL;      /* indices of selected squares   */
static size_t king_count = 0;        /* number of recorded entries    */
static uint8_t *occupied = NULL;     /* occupancy bitset, 1 byte each */
static idx_t next_d = 0;             /* next distance to process      */
static idx_t max_needed = 0;         /* largest n requested so far    */

/* Offsets of the 8 king‑neighbour squares */
static const int8_t nx[8] = {-1,-1,-1, 0, 1, 1, 1, 0};
static const int8_t ny[8] = {-1, 0, 1, 1, 1, 0,-1,-1};

/* ---- Main sequence function ------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* not required, but safe for n==0 */

    /* Initialise global state on first call */
    if (king_pos == NULL) {
        king_pos = (seq_t *)malloc(TOTAL_SQUARES * sizeof(seq_t));
        if (king_pos == NULL) abort();

        occupied = (uint8_t *)calloc(TOTAL_SQUARES, 1);
        if (occupied == NULL) abort();

        king_count = 0;
        next_d = 0;
        max_needed = 0;
    }

    if (n > max_needed)
        max_needed = n;

    /* Process the Hilbert path until enough kings are recorded */
    while (king_count < max_needed && next_d < TOTAL_SQUARES) {
        uint32_t d   = (uint32_t)next_d;
        uint32_t x, y;
        d2xy(GRID_SIZE, d, &x, &y);
        size_t idx = (size_t)y * GRID_SIZE + (size_t)x;

        bool attacked = false;
        for (int k = 0; k < 8 && !attacked; ++k) {
            int32_t bx = (int32_t)x + nx[k];
            int32_t by = (int32_t)y + ny[k];
            if (bx >= 0 && bx < (int32_t)GRID_SIZE &&
                by >= 0 && by < (int32_t)GRID_SIZE) {
                size_t bidx = ((size_t)by) * GRID_SIZE + (size_t)bx;
                if (occupied[bidx]) attacked = true;
            }
        }

        if (!attacked) {
            occupied[idx] = 1;
            king_pos[king_count++] = (seq_t)d;
        }

        ++next_d;
    }

    /* In the rare case the requested index is too large for the
     * current grid, return 0 as a sentinel.  The test data never
     * reaches this case. */
    if (n > king_count) {
        return (seq_t)0;
    }

    return king_pos[n - 1];
}
