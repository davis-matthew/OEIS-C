/*
 * A328909  Knight's tour on spirally numbered infinite board, when the knight
 *          always jumps on the unvisited square closest to the origin, first
 *          according to the sup-norm, then 2-norm, then number of the square:
 *          a(n) = number of square visited at move n.
 * https://oeis.org/A328909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 9915, u32 n <= 9915, u64 n <= 9915,
 *            u128 n <= 9915, big any n
 * Helpers:   llabs_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: simulate the knight's tour on an infinite board with a spiral
 * numbering of squares.  At each step the knight jumps to the unvisited
 * reachable square that is closest to the origin in sup‑norm; ties are
 * broken by Euclidean distance and then by the square number.
 * The function computes a(n) for any n ≥ 0.  The sequence stops when
 * no unvisited reachable square exists. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/llabs_val.h"

/* mapping of a square number to its (x,y) coordinates in the spiral.
 * True for the spiral described in OEIS A174344 (origin (0,0)=0, 1 at (1,0),
 * continuing counter‑clockwise). */
static inline idx_t index_from_coord(long long x, long long y)
{
    if (x == 0 && y == 0) return 0;
    long long ax = (x >= 0) ? x : -x;
    long long ay = (y >= 0) ? y : -y;
    long long r  = (ax > ay) ? ax : ay;   /* ring number */

    /* start index of ring r: (2r-1)^2 */
    __int128 tmp = (__int128)(2 * r - 1);
    idx_t start = (idx_t)(tmp * tmp);

    /* Determine offset on the ring */
    if (x == r && y >= -(r - 1) && y <= r) {                   /* right side */
        long long off = y - (-(r - 1));
        return start + off;
    } else if (y == r && x >= -r && x <= r - 1) {              /* top side */
        long long off = 2 * r + (r - 1 - x);
        return start + off;
    } else if (x == -r && y >= -r && y <= r - 1) {             /* left side */
        long long off = 4 * r + (r - 1 - y);
        return start + off;
    } else {                                                     /* bottom side */
        long long off = 6 * r + (x + r - 1);
        return start + off;
    }
}

/* dynamic array for visited square numbers */
static bool            *visited  = NULL;
static size_t          visited_cap = 0;

/* ensure the visited array can index up to idx */
static void ensure_visited_cap(idx_t idx)
{
    if (visited_cap > idx) return;
    size_t old = visited_cap;
    visited_cap = 1;
    while (visited_cap <= idx) visited_cap <<= 1;
    visited = (bool *)realloc(visited, visited_cap * sizeof(bool));
    memset(visited + old, 0, (visited_cap - old) * sizeof(bool));
}

/* structure for coordinates */
typedef struct { long long x; long long y; } PC;

/* dynamic array for the sequence values and their positions */
static seq_t  *seq    = NULL;
static PC     *pos    = NULL;
static size_t  seq_cap = 0;
static size_t  seq_len = 0;
static size_t  pos_cap = 0;

/* ensure capacity for seq and pos arrays */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap > needed) return;
    size_t old = seq_cap;
    seq_cap = 1;
    while (seq_cap <= needed) seq_cap <<= 1;
    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    pos = (PC *)realloc(pos, seq_cap * sizeof(PC));
}

/* append a new term to the sequence */
static void seq_append(seq_t val, long long x, long long y)
{
    ensure_seq_cap(seq_len);
    seq[seq_len] = val;
    pos[seq_len] = (PC){x, y};
    ensure_visited_cap((idx_t)val);
    visited[(size_t)val] = true;
    seq_len++;
}

/* initialize the sequence with the first term */
static void init_sequence(void)
{
    if (seq_len != 0) return;
    seq_append((seq_t)0, 0, 0);
}

/* compute the sequence up to index n (inclusive) */
static void compute_to(idx_t n)
{
    init_sequence();
    while (seq_len <= n) {
        long long cx = pos[seq_len - 1].x;
        long long cy = pos[seq_len - 1].y;

        /* knight move offsets */
        const int mvx[8] = { 2, 1, -1, -2, -2, -1,  1,  2};
        const int mvy[8] = { 1, 2,  2,  1, -1, -2, -2, -1};

        bool found = false;
        idx_t  best_num  = 0;
        long long best_sup  = 0;
        long long best_dist = 0;
        long long best_x = 0, best_y = 0;

        for (int i = 0; i < 8; ++i) {
            long long nx = cx + mvx[i];
            long long ny = cy + mvy[i];
            idx_t cand = index_from_coord(nx, ny);
            ensure_visited_cap(cand);
            if (visited[(size_t)cand]) continue;

            long long sup = llabs_val(nx) > llabs_val(ny) ? llabs_val(nx) : llabs_val(ny);
            long long dist = nx * nx + ny * ny;

            if (!found ||
                sup  < best_sup ||
                (sup == best_sup && (dist < best_dist ||
                 (dist == best_dist && cand < best_num)))) {
                found = true;
                best_num = cand;
                best_sup = sup;
                best_dist = dist;
                best_x = nx;
                best_y = ny;
            }
        }

        if (!found) break;   /* no more moves, sequence ends */
        seq_append(best_num, best_x, best_y);
    }
}

/* the required function */
seq_t a(idx_t n)
{
    compute_to(n);
    if (seq_len > n) return seq[n];
    /* If the sequence has ended before n, return the last known term. */
    return seq[seq_len - 1];
}
