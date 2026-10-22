/*
 * A340593  a(0) = 0; for n > 0, if n appears in the sequence then a(n) =
 *          a(n-1) - lastindex(n) if nonnegative and not already in the
 *          sequence, otherwise a(n) = a(n-1) + lastindex(n), where
 *          lastindex(n) is the index of the last appearance of n. Otherwise
 *          a(n) = a(n-1) - n if nonnegative and not already in th...
 * https://oeis.org/A340593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence for A340593. a(0) = 0 For n>0: if n has appeared in
 *            the sequence then step = lastindex(n) else step = n; if a(n-1)
 *            >= step and a(n-1)-step is not already in the sequence a(n) =
 *            a(n-1) - step; else a(n) = a(n-1) + step;
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- dynamic storage for the sequence ---------- */
static seq_t *seq_vals = NULL;    /* values a(0)..a(k) */
static idx_t  seq_len  = 0;       /* number of stored terms  */
static idx_t  seq_cap  = 0;       /* allocated capacity      */

/* ---------- map value -> last index ------------------- */
typedef struct {
    seq_t val;
    idx_t last;
} node_t;

static node_t *map_entries = NULL;    /* map table */
static size_t  map_len     = 0;       /* number of entries used */
static size_t  map_cap     = 0;       /* allocated table size   */

/* ensure capacity of seq_vals */
static void seq_ensure_cap(idx_t need)
{
    if (need <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap <= need) new_cap <<= 1;
    seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!seq_vals) abort();
    seq_cap = new_cap;
}

/* ensure capacity of map table */
static void map_ensure_cap(size_t need)
{
    if (need <= map_cap) return;
    size_t new_cap = map_cap ? map_cap * 2 : 16;
    while (new_cap <= need) new_cap <<= 1;
    map_entries = (node_t *)realloc(map_entries, new_cap * sizeof(node_t));
    if (!map_entries) abort();
    map_cap = new_cap;
}

/* linear search for value v in the map.  If found, *out_last is set. */
static bool map_find(seq_t v, idx_t *out_last)
{
    for (size_t i = 0; i < map_len; ++i) {
        if (map_entries[i].val == v) {
            if (out_last) *out_last = map_entries[i].last;
            return true;
        }
    }
    return false;
}

/* update the map: set or replace the last index of value v. */
static void map_update(seq_t v, idx_t idx)
{
    for (size_t i = 0; i < map_len; ++i) {
        if (map_entries[i].val == v) {
            map_entries[i].last = idx;
            return;
        }
    }
    /* not found – add a new entry */
    map_ensure_cap(map_len + 1);
    map_entries[map_len].val  = v;
    map_entries[map_len].last = idx;
    ++map_len;
}

/* initialise the sequence with a(0)=0 */
static void seq_init(void)
{
    if (seq_len) return;                 /* already initialised */
    seq_ensure_cap(1);
    seq_vals[0] = (seq_t)0;
    seq_len = 1;

    map_ensure_cap(1);
    map_entries[0].val  = (seq_t)0;
    map_entries[0].last = 0;
    map_len = 1;
}

/* ------------------------------------------------------- */
seq_t a(idx_t n)
{
    seq_init();

    if (n < seq_len) return seq_vals[n];

    /* generate terms up to n */
    for (idx_t i = seq_len; i <= n; ++i) {
        seq_t prev = seq_vals[i - 1];

        /* determine step size */
        idx_t last_idx = 0;
        seq_t step;
        if (map_find((seq_t)i, &last_idx)) {
            step = (seq_t)last_idx;
        } else {
            step = (seq_t)i;
        }

        /* try subtraction */
        bool try_sub = (prev >= step);
        seq_t candidate = 0;          /* meaningful only if try_sub */
        if (try_sub) candidate = prev - step;

        /* check if candidate already present */
        bool present = false;
        if (try_sub) present = map_find(candidate, &last_idx);

        seq_t next_val;
        if (try_sub && !present) {
            next_val = candidate;     /* subtract works */
        } else {
            next_val = prev + step;   /* otherwise add */
        }

        /* store and update */
        seq_ensure_cap(i + 1);
        seq_vals[i] = next_val;
        seq_len = i + 1;
        map_update(next_val, i);
    }

    return seq_vals[n];
}
