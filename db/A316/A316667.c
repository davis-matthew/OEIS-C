/*
 * A316667  Squares visited by a knight moving on a spirally numbered board
 *          always to the lowest available unvisited square.
 * https://oeis.org/A316667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, look
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 2016, u32 n <= 2016, u64 n <= 2016,
 *            u128 n <= 2016, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A316667: Knight moving on a spiral‑numbered board.
   a(1) = 1.
   Subsequent terms are obtained by repeatedly moving a knight to the
   lowest numbered unvisited square according to the square‑spiral numbering
   that starts with 1 at the origin. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------
   Helpers
   ------------------------------------------------------------------ */

/* Convert integer coordinates (x,y) into the spiral number.
   For the conventional spiral with 1 at (0,0), layers radiate outwards.
   idx_t is an unsigned 64‑bit integer, adequate for all intermediate
   values used by the board.  The function returns a seq_t value.  */
static inline seq_t pos_to_num(int64_t x, int64_t y)
{
    /* Layer index r = max(|x|,|y|) */
    uint64_t ax = (x >= 0) ? (uint64_t)x : (uint64_t)(-x);
    uint64_t ay = (y >= 0) ? (uint64_t)y : (uint64_t)(-y);
    uint64_t r  = ax > ay ? ax : ay;          /* layer index */

    /* (2r+1)^2 is the bottom‑right corner number of the layer */
    uint64_t base = 2 * r + 1;
    base *= base;                            /* base = (2r+1)^2 */

    uint64_t num;
    if (y == -(int64_t)r) {                  /* bottom side (moving left) */
        num = base - (r - (uint64_t)x);
    } else if (x == -(int64_t)r) {            /* left side (moving down) */
        num = base - (2 * r) - ((uint64_t)(y + (int64_t)r));
    } else if (y == (int64_t)r) {             /* top side (moving right) */
        num = base - (4 * r) - ((uint64_t)(x + (int64_t)r));
    } else {                                 /* right side (moving up) */
        num = base - (6 * r) - ((uint64_t)(r - (int64_t)y));
    }

    return (seq_t)num;
}

/* For the visited set we simply keep an array of 64‑bit keys
   where key = ((uint64_t) x << 32) | (uint32_t) y.  The array
   is large enough to hold all squares that could be visited for
   n <= 10000; it is re‑initialised for every call to a(). */

#define VMAX 65536UL
static uint64_t visited_keys[VMAX];

/* Linear search for a key in the first 'len' elements of visited_keys. */
static inline bool key_is_visited(uint64_t key, size_t len)
{
    for (size_t i = 0; i < len; ++i)
        if (visited_keys[i] == key)
            return true;
    return false;
}

/* Append a key to visited_keys; caller must ensure there is space. */
static inline void key_add(uint64_t key, size_t *len)
{
    visited_keys[*len] = key;
    ++(*len);
}

/* Pack coordinates into a 64‑bit key.  Both x and y are signed 32‑bit,
   but we store them as unsigned to avoid overflow in the shift. */
static inline uint64_t key_from_coords(int64_t x, int64_t y)
{
    uint64_t ux = (uint64_t)(uint32_t)x;
    uint64_t uy = (uint64_t)(uint32_t)y;
    return (ux << 32) | uy;
}

/* ------------------------------------------------------------------
   Main sequence function
   ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* The knight moves as on a chessboard. */
    static const int dx[8] = {  1,  2,  2,  1, -1, -2, -2, -1 };
    static const int dy[8] = {  2,  1, -1, -2, -2, -1,  1,  2 };

    /* Special case: the first term is always 1. */
    if (n <= 1)
        return (seq_t)1;

    /* Initialise the visited set and starting position. */
    size_t visited_len = 0;
    int64_t cur_x = 0, cur_y = 0;
    key_add(key_from_coords(cur_x, cur_y), &visited_len);

    /* Compute successive terms until the nth one is reached. */
    seq_t last_val = (seq_t)1;                 /* value of a(1) */
    for (idx_t step = 1; step < n; ++step)    /* step counts positions visited (starting at 1) */
    {
        seq_t best_val = 0;
        int64_t best_x = 0, best_y = 0;
        bool found = false;

        /* Examine all 8 knight moves. */
        for (int i = 0; i < 8; ++i)
        {
            int64_t nx = cur_x + dx[i];
            int64_t ny = cur_y + dy[i];
            uint64_t k = key_from_coords(nx, ny);
            if (key_is_visited(k, visited_len))
                continue;                    /* already visited */

            seq_t val = pos_to_num(nx, ny);
            if (!found || val < best_val)
            {
                best_val = val;
                best_x   = nx;
                best_y   = ny;
                found    = true;
            }
        }

        /* Move to the chosen square. */
        cur_x = best_x;
        cur_y = best_y;
        key_add(key_from_coords(cur_x, cur_y), &visited_len);

        last_val = best_val;                  /* value of a(step+1) */
    }

    return last_val;
}
