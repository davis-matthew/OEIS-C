/*
 * A333714  Squares visited by a chess king moving on a square-spiral numbered
 *          board where the king moves to the adjacent unvisited square
 *          containing the spiral number with the most divisors. In case of a
 *          tie it chooses the square with the highest spiral number.
 * https://oeis.org/A333714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 1114, u64 n <= 1114,
 *            u128 n <= 1114, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A333714: simulation of a king’s walk on a square‑spiral
   numbered board.  The king starts on square 1 and, at each step,
   moves to one of the eight neighbouring squares (king moves) that
   is unvisited and whose spiral number has the largest number of
   positive divisors; ties are broken by choosing the neighbour with
   the greater spiral number.  The walk ends when no unvisited
   neighbours remain.  This function returns the a(n) term for any
   n ≥ 1; for n larger than the finite length of the walk it returns
   the last visited value. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"

static const int64_t DX[8] = { 1,  1,  0, -1, -1, -1,  0,  1 };
static const int64_t DY[8] = { 0, -1, -1, -1,  0,  1,  1,  1 };

/* Convert (x,y) coordinates to the square‑spiral number.
   The spiral begins at 1 at (0,0) and expands counter‑clockwise. */
static seq_t spiral_num(int64_t x, int64_t y)
{
    int64_t ax = (x < 0) ? -x : x;
    int64_t ay = (y < 0) ? -y : y;
    int64_t r  = (ax > ay) ? ax : ay;   /* layer index */

    if (r == 0) return 1;

    /* first number in layer r */
    uint64_t start = (uint64_t)(2 * r - 1) * (uint64_t)(2 * r - 1) + 1ull;

    if (x == r && y != -r) {            /* right side, exclude bottom corner */
        uint64_t offset = (uint64_t)(y + r - 1);
        return (seq_t)(start + offset);
    }
    if (y == r && x != r) {             /* top side, exclude top‑right corner */
        uint64_t offset = (uint64_t)(r - 1 - x);
        return (seq_t)(start + 2 * r + offset);
    }
    if (x == -r && y != r) {            /* left side, exclude top‑left corner */
        uint64_t offset = (uint64_t)(r - 1 - y);
        return (seq_t)(start + 4 * r + offset);
    }
    /* bottom side (covers all remaining coordinates) */
    uint64_t offset = (uint64_t)(x + r - 1);
    return (seq_t)(start + 6 * r + offset);
}

/* Node stored in the walk: coordinates and spiral number */
struct node {
    int64_t x;
    int64_t y;
    seq_t  num;
};

#define MAX_WALK 2000    /* ample buffer for the 1113‑step walk */

static struct node walk[MAX_WALK];
static size_t walk_len = 0;
static bool walk_inited = false;

/* Initialise the walk with the starting square */
static void init_walk(void)
{
    if (walk_inited) return;
    walk_inited = true;
    walk_len = 1;
    walk[0].x = 0;
    walk[0].y = 0;
    walk[0].num = 1;
}

/* Test whether a spiral number has already been visited */
static bool visited(seq_t n)
{
    for (size_t i = 0; i < walk_len; ++i)
        if (walk[i].num == n) return true;
    return false;
}

/* Extend the walk until it reaches index n or cannot be extended */
static void extend_walk(idx_t n)
{
    while (walk_len < (size_t)n) {
        int64_t cx = walk[walk_len - 1].x;
        int64_t cy = walk[walk_len - 1].y;

        seq_t best_num = 0;
        int    best_div = -1;
        int    best_dir = -1;

        for (int d = 0; d < 8; ++d) {
            int64_t nx = cx + DX[d];
            int64_t ny = cy + DY[d];
            seq_t cand = spiral_num(nx, ny);
            if (visited(cand)) continue;

            uint64_t cand_u = (uint64_t)cand;
            int div = (int)divisor_count_u64(cand_u);

            if (div > best_div || (div == best_div && cand > best_num)) {
                best_div = div;
                best_num = cand;
                best_dir = d;
            }
        }

        if (best_dir == -1) break;          /* no moves possible */

        walk[walk_len].x = walk[walk_len - 1].x + DX[best_dir];
        walk[walk_len].y = walk[walk_len - 1].y + DY[best_dir];
        walk[walk_len].num = best_num;
        ++walk_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* unused offset position */
    init_walk();
    if (walk_len < (size_t)n) extend_walk(n);
    if (n <= walk_len) return walk[n - 1].num;
    return walk[walk_len - 1].num;
}
