/*
 * A384877  Irregular triangle read by rows where row k lists the lengths of
 *          maximal anti-runs (increasing by more than 1) in the binary
 *          indices of n.
 * https://oeis.org/A384877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (description):
 *  a(n) is the (n+1)-th element of the concatenation of the lengths
 *  of the maximal anti‑runs (indices of 1‑bits of successive integers,
 *  starting from 1) read row by row.  That is,
 *
 *      a = concat( len(run) for run in anti_runs(i) )
 *                     i = 1, 2, 3, … .
 *
 *  (An anti‑run is a maximal sequence of indices such that each
 *  successive index exceeds the previous one by more than 1.)
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* cached sequence terms                */
static size_t cache_size = 0;        /* number of terms currently cached     */
static size_t cache_cap   = 0;        /* current capacity of cache array      */
static idx_t  current_m   = 1;        /* next integer to process (row number) */

/* Append a value to the cache, enlarging the array if necessary. */
static void cache_append(seq_t val)
{
    if (cache_size >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return;                /* allocation failure: ignore */
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache       = new_cache;
        cache_cap   = new_cap;
    }
    cache[cache_size++] = val;
}

/* Process one integer m and append its anti‑run lengths to the cache. */
static void process_row(idx_t m)
{
    idx_t bits = m;
    seq_t run_len = 0;
    idx_t last_pos = 0;        /* 1‑based index of last 1‑bit encountered */
    idx_t pos = 0;             /* bit position (0‑based) */

    while (bits) {
        if (bits & 1u) {
            idx_t cur_pos = pos + 1;   /* convert to 1‑based index */
            if (run_len == 0) {        /* first run starts here */
                run_len = 1;
            } else if (cur_pos - last_pos > 1) {
                /* same run continues */
                run_len += 1;
            } else {
                /* run breaks; record previous run */
                cache_append((seq_t)run_len);
                run_len = 1;
            }
            last_pos = cur_pos;
        }
        bits >>= 1u;
        pos++;
    }
    if (run_len > 0) {
        cache_append((seq_t)run_len);
    }
}

/* Ensure the cache contains at least n+1 terms (indices 0 … n). */
static void ensure_cache(idx_t n)
{
    while (cache_size <= (size_t)n) {
        /* Skip n = 0 row (empty, no terms).  Start from 1. */
        if (current_m == 0) current_m = 1;
        process_row(current_m);
        current_m++;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
