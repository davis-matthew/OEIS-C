/*
 * A317416  Squares visited by a (1,4)-leaper (or giraffe) on a spirally
 *          numbered board and moving to the lowest available unvisited square
 *          at each step, squares labelled >=1.
 * https://oeis.org/A317416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = A317415(n-1) + 1 (simulation of a (1,4)-leaper, also
 *            called a giraffe) The algorithm keeps track of the leaper’s
 *            position and a hash set of the square numbers already visited.
 *            For each step it examines the eight possible leaper moves,
 *            chooses the reachable square with the lowest spiral num...
 * Range:     u8 none, u16 none, u32 n <= 9634, u64 n <= 9634, u128 n <= 9634,
 *            big any n
 * Helpers:   number_at_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required includes for C/C++ compatibility */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/number_at_coords.h"

/* -------------------------------------------------------------------------- */
/*  Parameters for the hash set.  The table size is a power of two so that
    the hash key can be reduced by masking.  */
#define HASH_SIZE 262144          /* >= 2 * maximum number of visited squares */
#define HASH_MASK (HASH_SIZE-1)

/* -------------------------------------------------------------------------- */
/*  Static state used for memoisation and for the hash set       */
static idx_t    computed_up_to = 0;         /* number of terms already computed */
static size_t   seq_cache_cap  = 0;         /* capacity of seq_cache         */
static seq_t   *seq_cache       = NULL;     /* array of computed sequence terms */

static seq_t   *hash_keys       = NULL;     /* keys for hash set (board numbers)   */
static uint8_t *hash_used       = NULL;     /* flags for hash slots               */
static size_t   hash_used_count = 0;         /* elements in the hash set           */

/* -------------------------------------------------------------------------- */
/*  Current position of the leaper (x, y) in integer coordinates.  The leaper
    moves in eight directions: (±1, ±4) and (±4, ±1).  */
static int64_t cur_x = 0, cur_y = 0;

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*  Basic hash table functions (open addressing with linear probing).         */
/*  The hash key is the board number `val`; the hash value is `val % HASH_SIZE`.  */

static void hash_init(void)
{
    /* Allocate the key array and used flag array.  Cast from void* to the
       appropriate pointer type so the code compiles under C++ as well. */
    hash_keys = (seq_t *)malloc(HASH_SIZE * sizeof(seq_t));
    hash_used = (uint8_t *)calloc(HASH_SIZE, sizeof(uint8_t));
    hash_used_count = 0;
}

static bool hash_contains(seq_t val)
{
    size_t idx = (size_t)(val % (seq_t)HASH_SIZE);
    while (hash_used[idx]) {
        if (hash_keys[idx] == val) return true;
        idx = (idx + 1) & HASH_MASK;
    }
    return false;
}

static void hash_insert(seq_t val)
{
    if (hash_contains(val)) return;
    size_t idx = (size_t)(val % (seq_t)HASH_SIZE);
    while (hash_used[idx]) {
        idx = (idx + 1) & HASH_MASK;
    }
    hash_keys[idx] = val;
    hash_used[idx] = 1;
    ++hash_used_count;
}

/* -------------------------------------------------------------------------- */
/*  Ensure that `seq_cache` has room for `cap` elements.  Resize by doubling
    the capacity until it fits.  */
static void ensure_cache_capacity(size_t cap)
{
    if (seq_cache_cap >= cap) return;
    size_t new_cap = (seq_cache_cap == 0) ? (cap > 0 ? cap : 1) : seq_cache_cap * 2;
    while (new_cap < cap) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    seq_cache = new_cache;
    seq_cache_cap = new_cap;
}

/* -------------------------------------------------------------------------- */
/*  Main function.  Computes a(n) for any n >= 1, memoising intermediate
    results.  Each call is O(1) after the required terms have been
    pre‑computed.  */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined, defensive */

    /* ---------- first time initialisation -------------------------------- */
    if (!hash_keys) {
        hash_init();
        ensure_cache_capacity(128);

        /* Starting position: (0,0) → square 1 */
        cur_x = 0;
        cur_y = 0;

        seq_t first = (seq_t)1;
        seq_cache[0] = first;
        hash_insert(first);
        computed_up_to = 1;
    }

    /* ---------- extend the sequence up to the requested index ----------- */
    while (computed_up_to < n) {

        /* Enumerate the eight possible leaper moves */
        static const int dx[8] = {  1,  1,  4,  4, -1, -1, -4, -4 };
        static const int dy[8] = {  4, -4,  1, -1,  4, -4,  1, -1 };

        seq_t   best_val = (seq_t)0;
        int64_t best_x   = 0;
        int64_t best_y   = 0;
        bool    first_candidate = true;

        for (int i = 0; i < 8; ++i) {
            int64_t nx = cur_x + dx[i];
            int64_t ny = cur_y + dy[i];
            seq_t    val = number_at_coords(nx, ny);
            if (!hash_contains(val)) {
                if (first_candidate || val < best_val) {
                    best_val = val;
                    best_x   = nx;
                    best_y   = ny;
                    first_candidate = false;
                }
            }
        }

        /* Append the chosen square to the cache */
        ensure_cache_capacity(computed_up_to + 1);
        seq_cache[computed_up_to] = best_val;
        hash_insert(best_val);

        /* Update current position */
        cur_x = best_x;
        cur_y = best_y;
        ++computed_up_to;
    }

    return seq_cache[n - (idx_t)1];
}
