/*
 * A274641  Counterclockwise square spiral constructed by greedy algorithm, so
 *          that each row, column, and diagonal contains distinct numbers.
 *          Start with 0 (so in this version a(n) = A274640(n) - 1).
 * https://oeis.org/A274641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   this function computes the sequence A274641 by simulating the
 *            counter‑clockwise square spiral constructed by a greedy
 *            algorithm. Each row, column and diagonal must contain distinct
 *            numbers. The algorithm proceeds step by step, always inserting
 *            the smallest non‑present integer that does not appear...
 * Range:     u8 n <= 8006, u16 n <= 8006, u32 n <= 8006, u64 n <= 8006,
 *            u128 n <= 8006, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Parameters for the bounding box of the spiral.  The maximum radius
   required for n <= 10000 is 140, therefore an offset of 200 is
   more than sufficient.  The macro values are compile‑time constants.  */
#define OFFSET 200
#define DIM    (OFFSET*2 + 1)

/* --------------------------------------------------------------------- */
/* Small integer set implementation (uses idx_t internally).  This type
   is POD, so it can safely be handled with malloc/realloc even
   when seq_t is a non‑trivial arbitrary‑precision integer type. */
typedef struct {
    idx_t *vals;
    size_t count;
    size_t cap;
} IntSet;

/* Create a new empty IntSet */
static IntSet *set_int_init(void)
{
    IntSet *s = (IntSet *)malloc(sizeof(IntSet));
    if (!s) abort();
    s->vals = NULL;
    s->count = 0;
    s->cap = 0;
    return s;
}

/* Append a value to the set (values are assumed distinct). */
static void set_int_add(IntSet *s, idx_t v)
{
    if (s->count == s->cap) {
        size_t newcap = s->cap ? s->cap * 2 : 8;
        idx_t *tmp = (idx_t *)realloc(s->vals, newcap * sizeof(idx_t));
        if (!tmp) abort();
        s->vals = tmp;
        s->cap = newcap;
    }
    s->vals[s->count++] = v;
}

/* Test membership of v in set s. */
static bool set_int_contains(const IntSet *s, idx_t v)
{
    for (size_t i = 0; i < s->count; ++i)
        if (s->vals[i] == v) return true;
    return false;
}

/* --------------------------------------------------------------------- */
/* Global data for the spiral walk */
static IntSet *rows[DIM];
static IntSet *cols[DIM];
static IntSet *diag1[DIM];
static IntSet *diag2[DIM];

static idx_t *cache = NULL;
static size_t cache_size = 0;
static idx_t cache_max = (idx_t)-1;   /* indicator of uninitialised cache */

static bool initialized = false;

/* Current position and direction of the walker */
static int  walk_x = 0, walk_y = 0;
static int  dir = 0;          /* 0:right, 1:up, 2:left, 3:down */
static int  step_len = 1;
static int  steps_left = 1;
static int  seg_repeat = 0;

static const int dx[4] = { 1, 0, -1, 0 };
static const int dy[4] = { 0, 1, 0, -1 };

/* --------------------------------------------------------------------- */
/* Lazily initialise the internal structures. */
static void init(void)
{
    if (initialized) return;

    for (int i = 0; i < DIM; ++i) {
        rows[i]  = set_int_init();
        cols[i]  = set_int_init();
        diag1[i] = set_int_init();
        diag2[i] = set_int_init();
    }

    /* place the first cell (0,0) with value 0 */
    set_int_add(rows[OFFSET], 0);
    set_int_add(cols[OFFSET], 0);
    set_int_add(diag1[OFFSET], 0);
    set_int_add(diag2[OFFSET], 0);

    /* initialise cache */
    cache_size = 64;
    cache = (idx_t *)calloc(cache_size, sizeof(idx_t));
    if (!cache) abort();
    cache[0] = 0;
    cache_max = 0;

    /* walker state */
    walk_x = 0; walk_y = 0;
    dir = 0;
    step_len = 1;
    steps_left = 1;
    seg_repeat = 0;

    initialized = true;
}

/* Ensure that 'cache' can hold at least 'need' elements. */
static void expand_cache(idx_t need)
{
    if (cache_size > (size_t)need) return;
    size_t new_size = cache_size * 2;
    while (new_size <= (size_t)need) new_size *= 2;
    idx_t *tmp = (idx_t *)realloc(cache, new_size * sizeof(idx_t));
    if (!tmp) abort();
    cache = tmp;
    cache_size = new_size;
}

/* Advance the walker by one step, fill the new cell with the smallest
   integer not yet present in its row, column, or either diagonal. */
static void step(void)
{
    /* move to next coordinate */
    walk_x += dx[dir];
    walk_y += dy[dir];
    --steps_left;
    if (steps_left == 0) {
        ++seg_repeat;
        dir = (dir + 1) & 3;
        if (seg_repeat == 2) {         /* after two segments, increase length */
            seg_repeat = 0;
            ++step_len;
        }
        steps_left = step_len;
    }

    /* find the smallest non‑present integer */
    idx_t cand = 0;
    for (;;) {
        bool in_row = set_int_contains(rows[OFFSET + walk_y], cand);
        bool in_col = set_int_contains(cols[OFFSET + walk_x], cand);
        bool in_d1  = set_int_contains(diag1[OFFSET + (walk_y - walk_x)], cand);
        bool in_d2  = set_int_contains(diag2[OFFSET + (walk_y + walk_x)], cand);
        if (!in_row && !in_col && !in_d1 && !in_d2) break;
        ++cand;
    }

    /* record the new value */
    set_int_add(rows[OFFSET + walk_y], cand);
    set_int_add(cols[OFFSET + walk_x], cand);
    set_int_add(diag1[OFFSET + (walk_y - walk_x)], cand);
    set_int_add(diag2[OFFSET + (walk_y + walk_x)], cand);

    /* store in cache */
    expand_cache(cache_max + 1);
    cache[++cache_max] = cand;
}

/* --------------------------------------------------------------------- */
/* Public entry point */
seq_t a(idx_t n)
{
    init();                 /* allocate and initialise all data structures */

    while (cache_max < n)
        step();              /* produce values up to n */

    return (seq_t)cache[n];
}
