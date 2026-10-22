/*
 * A362373  a(0) = 0; for n > 0, if n appears in the sequence then a(n) is the
 *          sum of the indices of all previous appearances of n. Otherwise
 *          a(n) = a(n-1) - n if nonnegative and not already in the sequence,
 *          otherwise a(n) = a(n-1) + n.
 * https://oeis.org/A362373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 9548, u32 n <= 9548, u64 n <= 9548,
 *            u128 n <= 9548, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A362373 (modified look‑and‑say)
 *  a(0) = 0;
 *  For n > 0:
 *    (1) If n occurs among {a(k) : k < n}, then
 *        a(n) = ∑_{k<n , a(k)=n} k        // sum of indices of previous appearances
 *    (2) Otherwise let c = a(n‑1) – n.
 *        If c ≥ 0  and  c has not occurred earlier, then a(n) = c
 *        else a(n) = a(n‑1) + n
 *
 *  The implementation keeps a cache of terms and a hash‑like table
 *  storing the current sum of indices for each value that has appeared.
 *  It computes terms on demand and is thread‑safe only for single‑thread
 *  usage (the problem guarantees single threaded calls).
 */

#include <stdlib.h>

/* Map entry: value → cumulative sum of all indices where the value has appeared */
typedef struct {
    seq_t value;
    seq_t sum;
} MapEntry;

static seq_t *seq = NULL;          /* array of computed terms */
static size_t seq_len = 0;         /* number of terms currently stored */
static size_t seq_cap = 0;         /* capacity of seq array */

static MapEntry *map = NULL;       /* array of map entries (value → sum) */
static size_t map_len = 0;         /* number of distinct values known */
static size_t map_cap = 0;         /* capacity of map array */

/* Ensure seq has at least min_cap elements */
static void seq_ensure(size_t min_cap) {
    if (min_cap > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Ensure map has at least min_cap entries */
static void map_ensure(size_t min_cap) {
    if (min_cap > map_cap) {
        size_t new_cap = map_cap ? map_cap * 2 : 8;
        if (new_cap < min_cap) new_cap = min_cap;
        map = (MapEntry *)realloc(map, new_cap * sizeof(MapEntry));
        map_cap = new_cap;
    }
}

/* Find map entry for a value; NULL if not present */
static MapEntry *find_entry(seq_t val) {
    for (size_t i = 0; i < map_len; ++i) {
        if (map[i].value == val) return &map[i];
    }
    return NULL;
}

/* Add index to the sum of a value (creating entry if needed) */
static void add_or_update(seq_t val, idx_t idx) {
    MapEntry *e = find_entry(val);
    if (e) {
        e->sum += (seq_t)idx;
    } else {
        map_ensure(map_len + 1);
        map[map_len].value = val;
        map[map_len].sum   = (seq_t)idx;
        ++map_len;
    }
}

/* Lazy initialization of first term and map entry (a(0) = 0) */
static void init_sequence(void) {
    if (seq_len == 0) {
        seq_ensure(1);
        seq[0] = (seq_t)0;
        seq_len = 1;

        map_ensure(1);
        map[0].value = (seq_t)0;
        map[0].sum   = (seq_t)0;
        map_len = 1;
    }
}

/* Public accessor for a(n) */
seq_t a(idx_t n) {
    init_sequence();

    /* Compute terms up to n if they haven't been computed yet */
    while (seq_len <= (size_t)n) {
        idx_t cur = (idx_t)seq_len;     /* the index we are computing */
        seq_t val;

        /* (1) Check if cur itself has appeared before */
        MapEntry *e_cur = find_entry((seq_t)cur);
        if (e_cur) {
            /* sum of indices of previous appearances of cur */
            val = e_cur->sum;
        } else {
            /* (2) Compute candidate a(n-1)-n if non‑negative and unused */
            seq_t prev = seq[seq_len - 1];
            if (prev >= (seq_t)cur) {
                seq_t cand = prev - (seq_t)cur;
                if (!find_entry(cand)) {
                    val = cand;
                } else {
                    val = prev + (seq_t)cur;
                }
            } else {
                /* prev < cur ⇒ candidate would be negative */
                val = prev + (seq_t)cur;
            }
        }

        /* Store result and update the map with this new occurrence */
        seq_ensure(seq_len + 1);
        seq[seq_len] = val;
        add_or_update(val, cur);
        ++seq_len;
    }

    return seq[(size_t)n];
}
