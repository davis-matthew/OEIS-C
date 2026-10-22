/*
 * A393466  a(n) is the distance between the A393465(n)-th point on a square
 *          spiral and the origin.
 * https://oeis.org/A393466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = distance from the origin of the n-th lattice point on the
 *            square spiral whose distance is an integer. The function lazily
 *            generates the integer‑distance points on the spiral and stores
 *            their distances. It works in O(n) time for each call and
 *            satisfies the type and header constraints
 * Range:     u8 n <= 1873, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper: integer square root for an unsigned 64‑bit integer.
   Works without floating point.  */
static inline idx_t isqrt_u64(idx_t v)
{
    if (v < 2) return v;
    idx_t r = 0;
    /* Find the highest power of 4 <= v.  */
    idx_t bit = ((idx_t)1) << 62;
    while (bit > v) bit >>= 2;
    /* Binary search.  */
    while (bit) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Dynamic array of already‑computed distances.  */
static seq_t *distances = NULL;
static size_t dist_cap   = 0;
static size_t dist_cnt   = 0;

/* Make sure the array can hold at least 'need' elements.  */
static void ensure_dist_capacity(size_t need)
{
    if (dist_cap >= need) return;
    size_t new_cap = dist_cap ? dist_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(distances, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(EXIT_FAILURE); /* fatal error */
    distances = new_ptr;
    dist_cap  = new_cap;
}

/* Main routine.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined; keeps type safety */

    /* On first call, initialise the table with the origin.  */
    if (dist_cnt == 0) {
        ensure_dist_capacity(16);
        distances[0] = 0;
        dist_cnt = 1;
    }

    /* If the requested element is already known, return it.  */
    if (n <= dist_cnt) return distances[n-1];

    /* Variables for generating the spiral.  */
    static idx_t step_len   = 1;   /* current segment length   */
    static int   dir        = 0;   /* 0:right,1:up,2:left,3:down */
    static idx_t steps_in_dir = 0;
    static idx_t dir_changes = 0;
    static idx_t x = 0, y = 0;     /* current coordinates */
    static idx_t idx = 1;          /* index of current point */

    /* Continue generating until we have at least n distances.  */
    while (dist_cnt < n) {
        /* Advance one step along the spiral.  */
        switch (dir) {
            case 0: ++x; break;
            case 1: ++y; break;
            case 2: --x; break;
            case 3: --y; break;
        }
        ++idx;   /* new point has index idx */

        /* Check whether the new point has an integer distance.  */
        idx_t sq = (idx_t)x * (idx_t)x + (idx_t)y * (idx_t)y;
        idx_t r  = isqrt_u64(sq);
        if (r * r == sq) {
            ensure_dist_capacity(dist_cnt + 1);
            distances[dist_cnt++] = (seq_t)r;
        }

        /* Update direction information.  */
        ++steps_in_dir;
        if (steps_in_dir == step_len) {
            steps_in_dir = 0;
            dir = (dir + 1) & 3;
            ++dir_changes;
            if (dir_changes == 2) {
                dir_changes = 0;
                ++step_len;
            }
        }
    }

    /* Return the requested value.  */
    return distances[n-1];
}
