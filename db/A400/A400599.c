/*
 * A400599  Squares visited by a chess king moving on the square spiral,
 *          always moving to an adjacent unvisited square whose number has the
 *          smallest product of digits; ties are resolved by choosing the
 *          lowest number.
 * https://oeis.org/A400599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, fini, full, base, new
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 151, u32 n <= 151, u64 n <= 151,
 *            u128 n <= 151, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A400599:
   Squares visited by a chess king moving on the square spiral,
   always moving to an adjacent unvisited square whose number has
   the smallest product of digits; ties are resolved by choosing
   the lowest number.
   Sequence is finite; a(151) = 82. */

/* Standard headers required for the implementation */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Utility: digit product of a seq_t number */
#include "../utils/digit_product.h"

/* Directions: N, NE, E, SE, S, SW, W, NW */
static const int8_t dirs[8][2] = {
    { 0,  1}, { 1,  1}, { 1,  0}, { 1, -1},
    { 0, -1}, {-1, -1}, {-1,  0}, {-1,  1}
};

/* Convert (x,y) coordinates in the spiral to its number n >= 1. */
static seq_t coord_to_num(int x, int y)
{
    int k = x > 0 ? x : -x;
    if (y > k)       k = y;
    if (-y > k)      k = -y;                     /* k = max(|x|,|y|) */

    seq_t m = (seq_t)(2 * k + 1) * (seq_t)(2 * k + 1);   /* max number in layer k */

    if (x ==  k && y == -k)                /* bottom‑right corner */
        return m;

    if (y == -k)                          /* bottom edge, moving left */
        return m - (seq_t)(k - x);

    if (x == -k)                          /* left edge, moving up */
        return m - (seq_t)(2 * k + (y + k));

    if (y ==  k)                           /* top edge, moving right */
        return m - (seq_t)(4 * k + (x + k));

    /* right edge, moving down (excluding already handled corners) */
    return m - (seq_t)(6 * k + (k - y));
}

/* ---------- Memoised generation of the sequence ---------- */

#define MAX_TERMS  151
#define MAX_VISITED 5000       /* enough for all visited numbers */

static seq_t seq[MAX_TERMS];   /* stored terms, 0‑based */
static size_t seq_len = 0;     /* number of terms stored */

static bool visited[MAX_VISITED] = {0};

static int curx = 0, cury = 0;      /* current king coordinates */
static seq_t cur_num = 0;           /* current king square number */

static bool initialized = false;

/* Initialise state once */
static void init_sequence(void)
{
    if (initialized) return;

    curx   = 0;
    cury   = 0;
    cur_num = 1;

    seq[0]  = 1;
    seq_len = 1;

    visited[1] = true;

    initialized = true;
}

/* Generate terms up to index n (1‑based). */
static void generate_upto(idx_t n)
{
    if (n <= seq_len) return;           /* already have it */

    while (seq_len < n) {
        seq_t best_prod = (seq_t)-1;
        seq_t best_num = 0;
        int   best_x   = 0, best_y   = 0;

        for (int d = 0; d < 8; ++d) {
            int nx = curx + dirs[d][0];
            int ny = cury + dirs[d][1];
            seq_t num = coord_to_num(nx, ny);

            if (num >= (seq_t)MAX_VISITED) continue;   /* guard against out‑of‑range numbers */

            if (visited[(size_t)num]) continue;           /* already visited */

            seq_t prod = digit_product(num);

            if (best_prod == (seq_t)-1 ||
                prod < best_prod ||
                (prod == best_prod && num < best_num)) {
                best_prod = prod;
                best_num  = num;
                best_x    = nx;
                best_y    = ny;
            }
        }

        if (best_prod == (seq_t)-1) break;   /* no unvisited neighbors */

        /* Accept best move */
        visited[(size_t)best_num] = true;
        seq[seq_len++] = best_num;
        cur_num = best_num;
        curx = best_x;
        cury = best_y;
    }
}

/* Public API required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1; guard */

    init_sequence();
    generate_upto(n);

    if (n <= seq_len)
        return seq[n-1];

    /* Sequence is finite; n beyond its length is undefined. */
    return 0;
}
