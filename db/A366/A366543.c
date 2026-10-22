/*
 * A366543  a(0) = 0; for n > 0, a(n) is the largest distance squared on a
 *          square spiral between any two previous occurrences of a(n-1). If
 *          a(n-1) has not previously occurred then a(n) = 0.
 * https://oeis.org/A366543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 182, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): a(0)=0; for n>0, a(n) is the largest squared Euclidean distance
   between any two previous occurrences of a(n‑1) in the standard square
   spiral.  If a(n‑1) has not occurred before, a(n)=0.   */

/* The implementation uses lazy, incremental computation with memoization
   and dynamic arrays.  It works in both C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- Internal data structures ------------------------------------------- */

typedef struct {
    seq_t value;          /* the sequence value this entry represents    */
    size_t occ;           /* number of occurrences collected             */
    seq_t max_dist;       /* largest squared distance among occurrences  */
    size_t cap;           /* capacity of xs/ys arrays                    */
    int64_t *xs;          /* x coordinates of all occurrences           */
    int64_t *ys;          /* y coordinates of all occurrences           */
} ValueEntry;

/* --- Global storage ------------------------------------------- */

static seq_t *a_cache   = NULL;   /* computed a(n) values */
static size_t a_cache_cap = 0;
static size_t a_count = 0;        /* number of values currently in a_cache */

static int64_t *coord_x   = NULL; /* x coordinate of index i */
static int64_t *coord_y   = NULL; /* y coordinate of index i */
static size_t coord_cap   = 0;
static size_t coord_count = 0;    /* number of coordinates stored (x[0]..x[coord_count-1]) */

static size_t coord_state_next = 1;      /* next index to generate */
static int   coord_state_dir   = 0;      /* 0 east,1 north,2 west,3 south */
static int   coord_state_len   = 1;      /* current step length */
static int   coord_state_steps_in_dir = 0;
static int   coord_state_dirs_done_in_len = 0;
static int64_t coord_state_curx = 0;
static int64_t coord_state_cury = 0;

static ValueEntry *value_entries = NULL;
static size_t value_entries_cap = 0;
static size_t value_entries_count = 0;

/* --- Utility functions ------------------------------------------- */

/* Ensure coordinate arrays have room and coordinates computed up to idx */
static void ensure_coords_up_to(idx_t idx)
{
    while (coord_state_next <= idx) {
        /* grow coordinate arrays if necessary */
        if (coord_count == coord_cap) {
            size_t new_cap = coord_cap ? coord_cap * 2 : 1;
            int64_t *tmp_x = (int64_t *)realloc(coord_x, new_cap * sizeof(int64_t));
            int64_t *tmp_y = (int64_t *)realloc(coord_y, new_cap * sizeof(int64_t));
            if (!tmp_x || !tmp_y) abort();
            coord_x = tmp_x;
            coord_y = tmp_y;
            coord_cap = new_cap;
        }

        /* step in the current direction */
        switch (coord_state_dir) {
            case 0: coord_state_curx += 1; break; /* east  */
            case 1: coord_state_cury += 1; break; /* north */
            case 2: coord_state_curx -= 1; break; /* west  */
            case 3: coord_state_cury -= 1; break; /* south */
        }

        /* store the coordinate */
        coord_x[coord_state_next] = coord_state_curx;
        coord_y[coord_state_next] = coord_state_cury;

        /* update state counters */
        coord_state_steps_in_dir++;
        if (coord_state_steps_in_dir == coord_state_len) {
            coord_state_steps_in_dir = 0;
            coord_state_dir = (coord_state_dir + 1) & 3;
            coord_state_dirs_done_in_len++;
            if (coord_state_dirs_done_in_len == 2) {
                coord_state_dirs_done_in_len = 0;
                coord_state_len++;
            }
        }

        coord_state_next++;
        coord_count++;
    }
}

/* Find a ValueEntry for a given value, or create it if it doesn't exist */
static ValueEntry *get_value_entry(seq_t val)
{
    for (size_t i = 0; i < value_entries_count; ++i) {
        if (value_entries[i].value == val) return &value_entries[i];
    }

    /* grow the values array if needed */
    if (value_entries_count == value_entries_cap) {
        size_t new_cap = value_entries_cap ? value_entries_cap * 2 : 4;
        ValueEntry *tmp = (ValueEntry *)realloc(value_entries, new_cap * sizeof(ValueEntry));
        if (!tmp) abort();
        value_entries = tmp;
        value_entries_cap = new_cap;
    }

    /* initialize the new entry */
    ValueEntry *e = &value_entries[value_entries_count++];
    e->value     = val;
    e->occ       = 0;
    e->max_dist  = 0;
    e->cap       = 4;
    e->xs = (int64_t *)malloc(4 * sizeof(int64_t));
    e->ys = (int64_t *)malloc(4 * sizeof(int64_t));
    if (!e->xs || !e->ys) abort();

    return e;
}

/* Add a new occurrence of a value at a particular index */
static void add_occurrence(seq_t val, idx_t idx)
{
    /* ensure the coordinates for this index exist */
    if ((size_t)idx >= coord_count) ensure_coords_up_to(idx);

    int64_t x = coord_x[idx];
    int64_t y = coord_y[idx];

    ValueEntry *e = get_value_entry(val);

    /* compute distance to all existing occurrences */
    for (size_t j = 0; j < e->occ; ++j) {
        int64_t dx  = x - e->xs[j];
        int64_t dy  = y - e->ys[j];
        int64_t ax = dx >= 0 ? dx : -dx;
        int64_t ay = dy >= 0 ? dy : -dy;
        seq_t dist_sq = (seq_t)ax * (seq_t)ax + (seq_t)ay * (seq_t)ay;
        if (dist_sq > e->max_dist) e->max_dist = dist_sq;
    }

    /* grow storage for this value if necessary */
    if (e->occ == e->cap) {
        size_t new_cap = e->cap * 2;
        int64_t *tmp_x = (int64_t *)realloc(e->xs, new_cap * sizeof(int64_t));
        int64_t *tmp_y = (int64_t *)realloc(e->ys, new_cap * sizeof(int64_t));
        if (!tmp_x || !tmp_y) abort();
        e->xs = tmp_x;
        e->ys = tmp_y;
        e->cap = new_cap;
    }

    /* store the new occurrence */
    e->xs[e->occ] = x;
    e->ys[e->occ] = y;
    e->occ++;
}

/* --- Main function ------------------------------------------- */

seq_t a(idx_t n)
{
    /* one‑time initialization of global data structures */
    static bool initialized = false;
    if (!initialized) {
        initialized = true;

        /* a(n) cache */
        a_cache_cap = 128;
        a_cache = (seq_t *)malloc(a_cache_cap * sizeof(seq_t));
        if (!a_cache) abort();
        a_count = 0;

        /* coordinate arrays */
        coord_cap = 1;
        coord_x = (int64_t *)malloc(coord_cap * sizeof(int64_t));
        coord_y = (int64_t *)malloc(coord_cap * sizeof(int64_t));
        if (!coord_x || !coord_y) abort();
        coord_x[0] = 0;
        coord_y[0] = 0;
        coord_count = 1; /* index 0 already set */

        /* value entries */
        value_entries_cap = 4;
        value_entries = (ValueEntry *)malloc(value_entries_cap * sizeof(ValueEntry));
        if (!value_entries) abort();
        value_entries_count = 0;
    }

    /* Grow a_cache if necessary */
    if (n >= a_cache_cap) {
        size_t new_cap = a_cache_cap;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = a_count; i <= n; ++i) {
        /* ensure coordinates exist */
        if (i > 0) ensure_coords_up_to(i);

        if (i == 0) {
            a_cache[0] = 0;
        } else {
            seq_t prev_val = a_cache[i - 1];
            ValueEntry *e = get_value_entry(prev_val);
            a_cache[i] = e->max_dist;
        }

        /* record this occurrence for future distance calculations */
        add_occurrence(a_cache[i], i);
    }

    a_count = n + 1;
    return a_cache[n];
}
