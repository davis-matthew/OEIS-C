/*
 * A265407  Spironacci-style recurrence: a(0)=0, a(1)=1, a(n) = 2*a(n) XOR
 *          a(A265409(n)).
 * https://oeis.org/A265407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(n)=2*a(n-1) XOR a(A265409(n))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implements the recurrence by generating the spiral
 *  positions of the sequence elements and using a hash table
 *  to find the nearest neighbour index A265409(n).  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* sizes for the hash map */
#define HASH_BITS   18
#define HASH_CAP    ((idx_t)1 << HASH_BITS)
#define HASH_MASK   (HASH_CAP - 1)
#define HASH_EMPTY  ((idx_t)UINT64_MAX)

/* --- global static state ------------------------------------ */

/* dynamic hash tables storing coordinate -> index */
static int  *hash_x = NULL;   /* size HASH_CAP */
static int  *hash_y = NULL;   /* size HASH_CAP */
static idx_t *hash_idx = NULL;/* size HASH_CAP */

/* arrays of already computed values and their coordinates */
static seq_t *vals   = NULL;  /* values a(n) */
static int   *xs     = NULL;  /* x coordinates of a(n) */
static int   *ys     = NULL;  /* y coordinates of a(n) */

/* capacity of the dynamically allocated arrays */
static size_t vals_cap = 0;    /* actual size of arrays above */

/* spiral generation state */
static int curX, curY;        /* last placed cell */
static int dir, step;         /* direction 0=E,1=N,2=W,3=S */
static int steps_left;        /* steps remaining in current leg */
static int twist;             /* toggle to increase step length */

/* number of values that have been generated (next index to compute) */
static idx_t curCount = 0;

/* forward declaration of helper */
static void init_once(void);

/* --- utility functions ------------------------------------- */

static void hash_init(void)
{
    for (idx_t i = 0; i < HASH_CAP; ++i) {
        hash_idx[i] = HASH_EMPTY;
    }
}

static void hash_put(int x, int y, idx_t id)
{
    idx_t h = ((idx_t)(x * 73856093u) ^ (idx_t)(y * 19349663u)) & HASH_MASK;
    while (hash_idx[h] != HASH_EMPTY) {
        h = (h + 1) & HASH_MASK;
    }
    hash_x[h]    = x;
    hash_y[h]    = y;
    hash_idx[h]  = id;
}

static idx_t hash_get(int x, int y)
{
    idx_t h = ((idx_t)(x * 73856093u) ^ (idx_t)(y * 19349663u)) & HASH_MASK;
    while (1) {
        if (hash_idx[h] == HASH_EMPTY) return HASH_EMPTY;
        if (hash_x[h] == x && hash_y[h] == y) return hash_idx[h];
        h = (h + 1) & HASH_MASK;
    }
}

static void ensure_vals_cap(size_t n)
{
    if (n <= vals_cap) return;
    size_t new_cap = vals_cap > 0 ? vals_cap : 32;
    while (new_cap < n + 1) new_cap <<= 1;
    seq_t *new_vals = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
    int   *new_xs   = (int   *)realloc(xs,   new_cap * sizeof(int));
    int   *new_ys   = (int   *)realloc(ys,   new_cap * sizeof(int));
    if (!new_vals || !new_xs || !new_ys) exit(1);
    vals   = new_vals;
    xs     = new_xs;
    ys     = new_ys;
    vals_cap = new_cap;
}

/* --- initialisation of global state ------------------------ */

static void init_once(void)
{
    if (curCount != 0) return;   /* already initialised */

    /* allocate hash tables */
    hash_x   = (int *)    malloc(HASH_CAP * sizeof(int));
    hash_y   = (int *)    malloc(HASH_CAP * sizeof(int));
    hash_idx = (idx_t *)  malloc(HASH_CAP * sizeof(idx_t));
    if (!hash_x || !hash_y || !hash_idx) exit(1);
    hash_init();

    /* allocate value tables */
    ensure_vals_cap(4);  /* small initial size */

    /* a(0) */
    vals[0] = (seq_t)0;
    xs[0]   = 0;
    ys[0]   = 0;
    hash_put(0, 0, 0);

    /* a(1) */
    vals[1] = (seq_t)1;
    xs[1]   = 1;
    ys[1]   = 0;
    hash_put(1, 0, 1);

    /* initialise spiral state after placing a(1) */
    curX = 1;  curY = 0;
    dir   = 1;             /* next direction is North (1) */
    step  = 1;             /* first leg length */
    steps_left = 1;        /* steps remaining in current leg */
    twist = 1;             /* will increase step after every two turns */

    curCount = 2;          /* next index to compute is 2 */
}

/* --- main sequence function -------------------------------- */

seq_t a(idx_t n)
{
    init_once();

    if ((size_t)n < curCount) return vals[n];

    ensure_vals_cap((size_t)n + 1);

    /* generate values from curCount up to n */
    for (idx_t i = curCount; i <= n; ++i) {
        /* move to next cell in spiral */
        static const int dx[4] = { +1, 0, -1, 0 };  /* E,N,W,S */
        static const int dy[4] = { 0, +1, 0, -1 };
        curX += dx[dir];
        curY += dy[dir];
        --steps_left;
        if (steps_left == 0) {
            dir = (dir + 1) & 3;          /* turn left */
            if (twist) step++;            /* increase step after 2 legs */
            twist ^= 1;
            steps_left = step;
        }

        xs[i] = curX;
        ys[i] = curY;
        hash_put(curX, curY, i);

        /* find the nearest neighbour that is not previous element */
        idx_t best_idx = HASH_EMPTY;
        unsigned best_dist = 0xFFFFFFFFu;

        for (int ddx = -1; ddx <= 1; ++ddx) {
            for (int ddy = -1; ddy <= 1; ++ddy) {
                if (ddx == 0 && ddy == 0) continue;
                idx_t nb = hash_get(curX + ddx, curY + ddy);
                if (nb == HASH_EMPTY) continue;
                if (nb == i-1) continue;  /* skip immediate predecessor */
                unsigned d2 = (unsigned)(ddx*ddx + ddy*ddy);
                if (d2 < best_dist || (d2 == best_dist && nb < best_idx)) {
                    best_dist = d2;
                    best_idx  = nb;
                }
            }
        }
        if (best_idx == HASH_EMPTY) best_idx = 0;   /* safety fallback */

        /* recurrence */
        vals[i] = ((seq_t)2 * vals[i-1]) ^ vals[best_idx];
    }

    curCount = n + 1;
    return vals[n];
}
