/*
 * A335900  Squares visited by a fairy chess wazir moving on a square-spiral
 *          numbered board where the wazir moves to the unvisited square
 *          containing the spiral number with the fewest divisors. In case of
 *          a tie it chooses the square with the lowest spiral number.
 * https://oeis.org/A335900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, fini, full
 * Formula:   sequence defined by simulating a fairy chess wazir walk on a *
 *            square‑spiral numbered board as described in OEIS entry A335900.
 *            The * wazir starts at square 1 (the centre), repeatedly moves to
 *            the * unvisited orthogonal neighbour whose number has the fewest
 *            divisors; * ties are broken by the lowe...
 * Range:     u8 n <= 34, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   spiral_from_coord
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/spiral_from_coord.h"

/* ------------------------------------------------------------------ */
/* helper: convert (x,y) in the square‑spiral to the linear number     */

/* ------------------------------------------------------------------ */
/* constants and static storage for the walk                          */
/* ------------------------------------------------------------------ */
#define VISIT_MAX   2048      /* safe upper bound for numbers seen    */
#define MAX_SEQ_LEN 200       /* the walk finishes in 61 steps        */

static bool          visited[VISIT_MAX] = { false };
static uint64_t      divcnt[VISIT_MAX] = { 0 };
static seq_t         seq[MAX_SEQ_LEN];
static size_t        seq_len = 0;

static const int dirs[4][2] = { { 1,  0 }, { -1, 0 }, { 0,  1 }, { 0, -1 } };

/* ------------------------------------------------------------------ */
/* compute divisor counts once                                            */
/* ------------------------------------------------------------------ */
static void compute_divisor_counts(void)
{
    for (size_t i = 1; i < VISIT_MAX; ++i) {
        for (size_t j = i; j < VISIT_MAX; j += i) {
            divcnt[j] += 1;
        }
    }
}

/* ------------------------------------------------------------------ */
/* build the entire sequence once                                        */
/* ------------------------------------------------------------------ */
static void build_sequence(void)
{
    if (seq_len > 0) return;  /* already built */

    compute_divisor_counts();

    int64_t x = 0, y = 0;       /* current position (centre) */
    seq[0] = (seq_t)1;
    seq_len = 1;
    visited[1] = true;

    for (;;) {
        uint64_t best_num = 0;
        uint64_t best_div = UINT64_MAX;
        int64_t  best_x  = 0;
        int64_t  best_y  = 0;
        bool found = false;

        for (int d = 0; d < 4; ++d) {
            int64_t nx = x + dirs[d][0];
            int64_t ny = y + dirs[d][1];
            uint64_t num = spiral_from_coord(nx, ny);

            if (num >= VISIT_MAX) continue;               /* safety guard */
            if (visited[(size_t)num]) continue;

            uint64_t dcnt = divcnt[(size_t)num];
            if (!found || dcnt < best_div || (dcnt == best_div && num < best_num)) {
                best_num = num;
                best_div = dcnt;
                best_x   = nx;
                best_y   = ny;
                found    = true;
            }
        }

        if (!found) break;          /* trapped – end of walk */

        /* make the move */
        x = best_x;
        y = best_y;
        visited[(size_t)best_num] = true;
        seq[seq_len++] = (seq_t)best_num;

        if (seq_len >= MAX_SEQ_LEN) break;   /* safety guard */
    }
}

/* ------------------------------------------------------------------ */
/* public entry point – retrieves a(n) for any n >= 1                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for offset 0 */

    build_sequence();                 /* ensure the sequence is built */

    if ((idx_t)seq_len >= n)          /* inside the finite sequence */
        return seq[(size_t)n - 1];
    else
        return 0;                     /* n beyond the finite sequence */
}
