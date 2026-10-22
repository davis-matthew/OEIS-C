/*
 * A336446  Squares visited by a chess queen moving on a square-spiral
 *          numbered board where the queen moves to an unvisited square
 *          containing the smallest prime number.
 * https://oeis.org/A336446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A336446: squares visited by a chess queen moving on a square‑spiral
   numbered board.  The queen starts on the square numbered 1.  At each step
   it moves like a chess queen (any distance in one of the eight directions)
   to an unvisited square containing the smallest prime number that can be
   reached without passing over a visited square.  If no such move exists the
   sequence terminates.  This function generates the sequence on demand and
   memoises the results. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static int   *x_coord   = NULL;   /* x coordinate of each number   */
static int   *y_coord   = NULL;   /* y coordinate of each number   */
static bool  *visited   = NULL;   /* visited flag for each number  */
static bool  *is_prime  = NULL;   /* prime flag for each number    */
static int   *coord2num = NULL;   /* mapping from coordinate to number */
static int    R, side;            /* radius and side length of grid */

static seq_t *seq_cache   = NULL; /* memoised sequence values      */
static size_t seq_cap     = 0, seq_len = 0;  /* current capacity / length */

/* Upper bound for numbers that can be visited – larger than the maximum
   visited number (79187).  100 000 is safely sufficient. */
#define MAX_NUM 100000

/* Ensure seq_cache has at least “needed” elements. */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Return the linear index in coord2num for a coordinate. */
static inline int idx_for_coord(int x, int y)
{
    return (x + R) * side + (y + R);
}

/* Pre‑compute coordinates, mapping, prime sieve, etc. */
static void initialize(void)
{
    /* Determine R such that (2*R+1)^2 > MAX_NUM. */
    R = 0;
    while (((2 * R + 1) * (2 * R + 1)) <= MAX_NUM) ++R;
    side = 2 * R + 1;                 /* grid is from -R .. R inclusive */

    /* Allocate arrays. */
    x_coord   = (int *)calloc(MAX_NUM + 1, sizeof(int));
    y_coord   = (int *)calloc(MAX_NUM + 1, sizeof(int));
    visited   = (bool *)calloc(MAX_NUM + 1, sizeof(bool));
    is_prime  = (bool *)calloc(MAX_NUM + 1, sizeof(bool));
    coord2num = (int *)calloc(side * side, sizeof(int));

    /* Prime sieve. */
    if (MAX_NUM >= 2) {
        for (int i = 2; i <= MAX_NUM; ++i) is_prime[i] = true;
        for (int p = 2; p * p <= MAX_NUM; ++p) {
            if (is_prime[p]) {
                for (int k = p * p; k <= MAX_NUM; k += p)
                    is_prime[k] = false;
            }
        }
    }

    /* Spiral generation. */
    int num = 1;
    x_coord[1] = 0;
    y_coord[1] = 0;
    coord2num[idx_for_coord(0, 0)] = 1;
    ++num; /* start next number */

    for (int r = 1; num <= MAX_NUM; ++r) {
        int cx = r;
        int cy = -(r - 1);

        /* Place the first square of this layer. */
        if (num > MAX_NUM) break;
        x_coord[num] = cx;
        y_coord[num] = cy;
        coord2num[idx_for_coord(cx, cy)] = num;
        ++num;

        /* Move upward 2r-1 steps. */
        for (int i = 0; i < 2 * r - 1 && num <= MAX_NUM; ++i) {
            ++cy;
            x_coord[num] = cx;
            y_coord[num] = cy;
            coord2num[idx_for_coord(cx, cy)] = num;
            ++num;
        }
        /* Move left 2r steps. */
        for (int i = 0; i < 2 * r && num <= MAX_NUM; ++i) {
            --cx;
            x_coord[num] = cx;
            y_coord[num] = cy;
            coord2num[idx_for_coord(cx, cy)] = num;
            ++num;
        }
        /* Move down 2r steps. */
        for (int i = 0; i < 2 * r && num <= MAX_NUM; ++i) {
            --cy;
            x_coord[num] = cx;
            y_coord[num] = cy;
            coord2num[idx_for_coord(cx, cy)] = num;
            ++num;
        }
        /* Move right 2r steps. */
        for (int i = 0; i < 2 * r && num <= MAX_NUM; ++i) {
            ++cx;
            x_coord[num] = cx;
            y_coord[num] = cy;
            coord2num[idx_for_coord(cx, cy)] = num;
            ++num;
        }
    }

    /* Initialise sequence with the starting square. */
    ensure_seq_capacity(1);
    seq_cache[0] = (seq_t)1;
    seq_len = 1;
    visited[1] = true;
}

/* Generate the next term of the sequence.
   Returns true on success, false when no further move is possible. */
static bool generate_next(void)
{
    int cur_num = (int)seq_cache[seq_len - 1];
    int cur_x   = x_coord[cur_num];
    int cur_y   = y_coord[cur_num];

    static const int dx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    static const int dy[8] = { 1, 1, 0, -1, -1, -1,  0,  1 };

    int best = 0;  /* best square number found */

    for (int d = 0; d < 8; ++d) {
        int dist = 1;
        while (1) {
            int nx = cur_x + dx[d] * dist;
            int ny = cur_y + dy[d] * dist;
            if (nx < -R || nx > R || ny < -R || ny > R) break;
            int idx = idx_for_coord(nx, ny);
            int val = coord2num[idx];
            if (val == 0) break;          /* no square at this coordinate */
            if (visited[val]) break;      /* cannot pass over or land on a visited square */
            if (is_prime[val]) {
                if (best == 0 || val < best) best = val;
                break;                  /* first prime along this direction is the smallest there */
            }
            ++dist;
        }
    }

    if (best == 0) return false;  /* no move possible */

    visited[best] = true;
    ensure_seq_capacity(seq_len + 1);
    seq_cache[seq_len] = (seq_t)best;
    ++seq_len;
    return true;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, but safeguard */

    static bool initialized = false;
    if (!initialized) {
        initialize();
        initialized = true;
    }

    /* If we already have the term, return it. */
    if ((idx_t)seq_len >= n) return seq_cache[n - 1];

    /* Generate further terms until we have enough or no move possible. */
    while ((idx_t)seq_len < n) {
        if (!generate_next()) break;  /* sequence finished */
    }

    if ((idx_t)seq_len >= n) return seq_cache[n - 1];
    /* Request beyond the finite sequence: return 0 (undefined). */
    return 0;
}
