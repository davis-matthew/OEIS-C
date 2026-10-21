/*
 * A274369  Let the starting square of Langton's ant have coordinates (0, 0),
 *          with the ant looking in negative x-direction. a(n) is the
 *          x-coordinate of the ant after n moves.
 * https://oeis.org/A274369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   computed by simulating Langton's ant. * Starting at (0,0)
 *            looking west. On a white cell the ant turns left *
 *            (counter‑clockwise), on a black cell it turns right. The ant *
 *            toggles the cell colour after observing it. * The function
 *            returns the x‑coordinate after n moves. * * This implementation
 *            us...
 * Range:     i8 n <= 9887, i16 n <= 9887, i32 n <= 9887, i64 n <= 9887,
 *            i128 n <= 9887, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define GRID_SIZE   4096          /* must be even */
#define CENTER      (GRID_SIZE/2) /* origin offset */

static uint8_t grid[GRID_SIZE][GRID_SIZE];
/* direction indices: 0 = west, 1 = north, 2 = east, 3 = south */
static const int dx[4] = {-1, 0, 1, 0};
static const int dy[4] = { 0, 1, 0,-1};

seq_t a(idx_t n)
{
    /* reset the grid */
    memset(grid, 0, sizeof(grid));

    int32_t x = 0, y = 0;
    int dir = 0;          /* start facing west */

    /* simulation loop */
    for (idx_t i = 0; i < n; ++i) {
        /* position in the grid */
        int gx = x + CENTER;
        int gy = y + CENTER;

        /* observe current cell colour */
        bool was_black = grid[gy][gx] != 0;

        /* toggle colour */
        grid[gy][gx] ^= 1;

        /* turn */
        dir = was_black ? (dir + 1) & 3 : (dir + 3) & 3;

        /* move forward */
        x += dx[dir];
        y += dy[dir];
    }

    return (seq_t)x;
}
