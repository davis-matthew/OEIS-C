/*
 * A270647  The sequence a of 1's and 2's starting with (2,2,1,2) such that
 *          a(n) is the length of the (n+3)rd run of a.
 * https://oeis.org/A270647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A270647:
   a(n) = length of the (n+3)rd run of a
   with initial terms a(1..4) = 2,2,1,2.
   Run lengths are 1 or 2, alternating values 2,1,2,1,...
   The sequence can be generated iteratively in O(n). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state across calls */
    static seq_t *cache = NULL;          /* stored sequence values (1‑based) */
    static idx_t cache_size = 0;         /* number of values stored */
    static idx_t cache_cap   = 0;         /* allocated capacity */
    static idx_t next_run_idx = 4;        /* run number of the current (or next) run */
    static seq_t cur_run_val = 0;        /* value of the current run */
    static idx_t cur_run_len = 0;        /* length of current run */
    static idx_t cur_run_pos = 0;        /* positions emitted in current run */

    /* Initialise on first use; add the first four terms */
    if (!cache) {
        /* initial capacity larger than 4 */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;           /* out of memory – unlikely in tests */
        cache[0] = 2;
        cache[1] = 2;
        cache[2] = 1;
        cache[3] = 2;
        cache_size = 4;
        next_run_idx = 4;               /* next run to start (run 4) */
        cur_run_len = 0;                /* triggers start of first run on demand */
        cur_run_pos = 0;
    }

    /* If the requested index is already known, return it */
    if (n <= cache_size && n >= 1)
        return cache[n - 1];

    /* Generate terms up to n */
    while (cache_size < n) {
        /* Start a new run if the current one is finished or not yet started */
        if (cur_run_pos >= cur_run_len) {
            /* If this is not the very first run, advance to the next run number */
            if (cur_run_len != 0)
                ++next_run_idx;

            /* Compute value for this run: odd runs are 2, even runs are 1 */
            cur_run_val = (next_run_idx & 1) ? 2 : 1;

            /* Compute length of this run */
            if (next_run_idx <= 3) {
                /* Initial run lengths (run 1 → 2, run 2 → 1, run 3 → 1) */
                cur_run_len = (next_run_idx == 1) ? (idx_t)2 : (idx_t)1;
            } else {
                /* For run i ≥ 4, length = a[i‑3] */
                cur_run_len = (idx_t)cache[next_run_idx - 4];
            }
            cur_run_pos = 0;
        }

        /* Ensure capacity for one more element */
        if (cache_size == cache_cap) {
            cache_cap <<= 1;   /* double capacity */
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) { free(cache); return 0; }
            cache = tmp;
        }

        /* Emit next element of current run */
        cache[cache_size] = cur_run_val;
        ++cache_size;
        ++cur_run_pos;
    }

    return cache[n - 1];
}
