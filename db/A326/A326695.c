/*
 * A326695  Langton's ant with three cell colors: color of the cell the ant
 *          moves to at the end of iteration n: 0 for white, 1 for black, 2
 *          for gray.
 * https://oeis.org/A326695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 276, u16 n <= 276, u32 n <= 276, u64 n <= 276,
 *            u128 n <= 276, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+276) = a(n).  The sequence is periodic with period 276.
   We compute the first 276 terms by simulating the three‑color Langton's
   ant described in the OEIS entry:
   - On white (0): turn right, paint black (1), move forward.
   - On black (1): turn left, paint gray (2), move forward.
   - On gray  (2): turn 180°, paint white (0), move forward.
   a(n) is the color of the cell the ant moves to at the end of iteration n. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>    /* for NULL and size_t */

#define PERIOD   276
#define MAX_CELLS 2048   /* sufficient for 276‑step simulation */

/* Cache of the first PERIOD terms */
static seq_t period_cache[PERIOD];
static bool initialized = false;

/* Simple in‑memory grid for visited cells */
typedef struct {
    int64_t x;
    int64_t y;
    seq_t color;
} Cell;

static Cell cells[MAX_CELLS];
static size_t cell_count = 0;

/* Retrieve a cell; create it with color 0 if it does not exist. */
static Cell *get_cell(int64_t x, int64_t y)
{
    for (size_t i = 0; i < cell_count; ++i) {
        if (cells[i].x == x && cells[i].y == y)
            return &cells[i];
    }
    if (cell_count >= MAX_CELLS) {
        /* This should never happen for the period simulation. */
        return NULL;
    }
    cells[cell_count].x = x;
    cells[cell_count].y = y;
    cells[cell_count].color = 0;
    ++cell_count;
    return &cells[cell_count - 1];
}

/* Return the a(n) value for any nonnegative index n. */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* Initialise the cache by simulating the first PERIOD steps. */
        int64_t x = 0, y = 0;
        unsigned dir = 0; /* 0:N, 1:E, 2:S, 3:W */

        /* Start with a single white cell at (0,0). */
        cell_count = 1;
        cells[0].x = 0;
        cells[0].y = 0;
        cells[0].color = 0;

        for (size_t step = 0; step < PERIOD; ++step) {
            Cell *cur = get_cell(x, y);
            seq_t c = cur->color;
            seq_t next_color;

            if (c == 0) {      /* white */
                dir = (dir + 1) & 3;   /* right */
                next_color = 1;
            } else if (c == 1) { /* black */
                dir = (dir + 3) & 3;   /* left */
                next_color = 2;
            } else {            /* gray */
                dir = (dir + 2) & 3;   /* 180° */
                next_color = 0;
            }
            cur->color = next_color;

            /* Move forward one step in the new direction. */
            if (dir == 0) ++y;
            else if (dir == 1) ++x;
            else if (dir == 2) --y;
            else if (dir == 3) --x;

            /* Record the color of the destination cell before any change. */
            Cell *dest = get_cell(x, y);
            period_cache[step] = dest->color;
        }
        initialized = true;
    }

    /* Return the cached value with periodicity. */
    size_t idx = (size_t)(n % PERIOD);
    return period_cache[idx];
}
