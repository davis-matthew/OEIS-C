/*
 * A343746  The x,y,z coordinates of the points visited by a knight on a 3D
 *          cubic lattice using the step rules given in A343678.
 * https://oeis.org/A343746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini
 * Formula:   not stated
 * Range:     i8 n <= 89, i16 n <= 89, i32 n <= 89, i64 n <= 89, i128 n <= 89,
 *            big any n
 * Helpers:   vis_hash
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the sequence A343746, the flattened
 * coordinates (x,y,z) of a knight traversing a 3‑D cubic lattice.
 * The path is defined by the step rules of A343678:
 *   1) choose a step to a point with the fewest visited neighbours
 *   2) among them, choose the one with the smallest squared distance
 *      from the origin
 *   3) among those, choose the one with the largest product of |x|,|y|,|z|
 *   4) finally break ties by minimal |x|, then |y|, then |z|
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/vis_hash.h"

/* Helpers ------------------------------------------------------------ */
static inline seq_t abs_seq(seq_t v) { return v < 0 ? -v : v; }

/* Knight moves: permutations of (±1, ±2, 0) */
static const int8_t moves[24][3] = {
    {+1,+2,0}, {+1,-2,0}, {-1,+2,0}, {-1,-2,0},
    {+2,+1,0}, {+2,-1,0}, {-2,+1,0}, {-2,-1,0},
    {+1,0,+2}, {+1,0,-2}, {-1,0,+2}, {-1,0,-2},
    {+2,0,+1}, {+2,0,-1}, {-2,0,+1}, {-2,0,-1},
    {0,+1,+2}, {0,+1,-2}, {0,-1,+2}, {0,-1,-2},
    {0,+2,+1}, {0,+2,-1}, {0,-2,+1}, {0,-2,-1}
};

/* A point in 3‑D space ---------------------------------------------- */
typedef struct { seq_t x; seq_t y; seq_t z; } Point;

/* Dynamic array of visited points ------------------------------------ */
static Point *visited_pts = NULL;
static size_t visited_cnt = 0;   /* number of points stored */
static size_t visited_cap = 0;   /* capacity of visited_pts */

/* Simple open‑addressing set for visited points ---------------------- */
typedef struct {
    seq_t x, y, z;
    unsigned char used;
} VisItem;

static VisItem *vis_table = NULL;
static size_t vis_table_size = 0;
static size_t vis_used = 0;

/* Resize the hash table ---------------------------------------------- */
static void vis_rehash(size_t new_size)
{
    VisItem *old   = vis_table;
    size_t   oldsz = vis_table_size;

    vis_table = (VisItem*)calloc(new_size, sizeof(VisItem));
    vis_table_size = new_size;

    for (size_t i = 0; i < oldsz; ++i) {
        if (old[i].used) {
            size_t idx = vis_hash(old[i].x, old[i].y, old[i].z) & (new_size - 1);
            while (vis_table[idx].used)
                idx = (idx + 1) & (new_size - 1);
            vis_table[idx] = old[i];
        }
    }
    free(old);
}

/* Check load factor and trigger resize -------------------------------- */
static void vis_check_resize(void)
{
    if (vis_used * 4 > vis_table_size * 3)
        vis_rehash(vis_table_size * 2);
}

/* Insert a point into the set ---------------------------------------- */
static void vis_insert(seq_t x, seq_t y, seq_t z)
{
    vis_check_resize();
    size_t idx = vis_hash(x, y, z) & (vis_table_size - 1);
    while (vis_table[idx].used) {
        if (vis_table[idx].x == x && vis_table[idx].y == y && vis_table[idx].z == z)
            return;
        idx = (idx + 1) & (vis_table_size - 1);
    }
    vis_table[idx].x = x;
    vis_table[idx].y = y;
    vis_table[idx].z = z;
    vis_table[idx].used = 1;
    ++vis_used;
}

/* Check whether a point is already visited ----------------------------- */
static bool vis_contains(seq_t x, seq_t y, seq_t z)
{
    size_t idx = vis_hash(x, y, z) & (vis_table_size - 1);
    while (vis_table[idx].used) {
        if (vis_table[idx].x == x && vis_table[idx].y == y && vis_table[idx].z == z)
            return true;
        idx = (idx + 1) & (vis_table_size - 1);
    }
    return false;
}

/* Ensure visited_pts has capacity for at least 'needed' elements ------- */
static void ensure_visited_cap(size_t needed)
{
    if (needed <= visited_cap) return;
    size_t new_cap = visited_cap ? visited_cap << 1 : 128;
    while (new_cap < needed) new_cap <<= 1;
    Point *tmp = (Point*)realloc(visited_pts, new_cap * sizeof(Point));
    if (!tmp) exit(1);
    visited_pts = tmp;
    visited_cap = new_cap;
}

/* Compute the number of visited neighbours of a point ----------------- */
static int neighbor_count(seq_t x, seq_t y, seq_t z)
{
    int cnt = 0;
    for (int i = 0; i < 24; ++i) {
        seq_t nx = x + (seq_t)moves[i][0];
        seq_t ny = y + (seq_t)moves[i][1];
        seq_t nz = z + (seq_t)moves[i][2];
        if (vis_contains(nx, ny, nz)) ++cnt;
    }
    return cnt;
}

/* Generate the path up to 'target_point' (0‑based) --------------------- */
static void simulate_up_to_point(idx_t target_point)
{
    while (visited_cnt <= target_point) {
        /* Current position */
        Point cur = visited_pts[visited_cnt - 1];

        /* Collect candidates */
        struct Candidate {
            seq_t x, y, z;
            int  vCount;
            seq_t dist; /* squared distance from origin */
            seq_t prod; /* |x|*|y|*|z| */
        };
        struct Candidate cand[24];
        int cand_cnt = 0;

        for (int i = 0; i < 24; ++i) {
            seq_t nx = cur.x + (seq_t)moves[i][0];
            seq_t ny = cur.y + (seq_t)moves[i][1];
            seq_t nz = cur.z + (seq_t)moves[i][2];
            if (vis_contains(nx, ny, nz)) continue;

            cand[cand_cnt].x = nx;
            cand[cand_cnt].y = ny;
            cand[cand_cnt].z = nz;
            cand[cand_cnt].vCount = neighbor_count(nx, ny, nz);
            cand[cand_cnt].dist = nx * nx + ny * ny + nz * nz;
            seq_t prod = abs_seq(nx);
            prod *= abs_seq(ny);
            prod *= abs_seq(nz);
            cand[cand_cnt].prod = prod;
            ++cand_cnt;
        }

        /* Pick the best candidate according to the rules */
        int best = 0;
        for (int i = 1; i < cand_cnt; ++i) {
            struct Candidate *c = &cand[i];
            struct Candidate *b = &cand[best];
            if (c->vCount < b->vCount) { best = i; }
            else if (c->vCount == b->vCount) {
                if (c->dist < b->dist) { best = i; }
                else if (c->dist == b->dist) {
                    if (c->prod > b->prod) { best = i; }
                    else if (c->prod == b->prod) {
                        if (abs_seq(c->x) < abs_seq(b->x)) { best = i; }
                        else if (abs_seq(c->x) == abs_seq(b->x)) {
                            if (abs_seq(c->y) < abs_seq(b->y)) { best = i; }
                            else if (abs_seq(c->y) == abs_seq(b->y)) {
                                if (abs_seq(c->z) < abs_seq(b->z)) { best = i; }
                            }
                        }
                    }
                }
            }
        }

        struct Candidate *sel = &cand[best];

        /* Store the selected point */
        ensure_visited_cap(visited_cnt + 1);
        visited_pts[visited_cnt].x = sel->x;
        visited_pts[visited_cnt].y = sel->y;
        visited_pts[visited_cnt].z = sel->z;
        ++visited_cnt;
        vis_insert(sel->x, sel->y, sel->z);
    }
}

/* One‑time initialisation --------------------------------------------- */
static void init_sequence(void)
{
    if (visited_pts) return;       /* already initialised */

    /* Allocate space for visited points */
    visited_cap = 128;
    visited_pts = (Point*)malloc(visited_cap * sizeof(Point));
    if (!visited_pts) exit(1);
    visited_cnt = 0;

    /* Insert the origin */
    visited_pts[0].x = 0;
    visited_pts[0].y = 0;
    visited_pts[0].z = 0;
    visited_cnt = 1;

    /* Initialise the hash set */
    vis_table_size = 16;
    vis_table = (VisItem*)calloc(vis_table_size, sizeof(VisItem));
    if (!vis_table) exit(1);
    vis_insert(0, 0, 0);
}

/* Public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined but safe */

    /* Determine which point we need and which coordinate */
    idx_t point_index = (n - 1) / 3;
    int  rem        = (int)(n % 3);

    init_sequence();
    simulate_up_to_point(point_index);

    if (rem == 1) return visited_pts[point_index].x;
    if (rem == 2) return visited_pts[point_index].y;
    return visited_pts[point_index].z; /* rem == 0 */
}
