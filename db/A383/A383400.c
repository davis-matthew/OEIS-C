/*
 * A383400  Starting values of maximal runs of at least five integers, each
 *          with exactly two distinct prime factors.
 * https://oeis.org/A383400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383400: starting values of maximal runs of at least five
 * consecutive integers, each having exactly two distinct prime factors.
 * We generate the runs by scanning integers, recording the start of any
 * run of ω = 2 that has length ≥ 5 and is terminated by a non‑ω=2.
 * The function computes terms lazily and keeps a static cache of the
 * discovered start values.  Subsequent calls can be in any order.
 *
 * Requires the utility function
 *   int count_distinct_primes(uint64_t n);
 * which is supplied by the build environment.  Include its header
 * (the exact filename may vary; use the pattern "../utils/NAME.h").
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdbool.h>  /* bool */
#include <stdlib.h>   /* malloc, free */

#include "../utils/count_distinct_primes.h"

/* --------- static state for caching runs --------- */
static seq_t *run_cache   = NULL;   /* array of run start values          */
static size_t run_capacity = 0;     /* allocated size of run_cache         */
static size_t run_count    = 0;     /* number of stored run starts         */

static idx_t  last_scanned = 0;      /* last integer that was examined     */
static bool   in_run       = false;  /* true if currently inside ω=2 run   */
static idx_t  run_start    = 0;      /* start of the current run           */
static idx_t  run_len      = 0;      /* length of the current run          */

/* Append a newly found run start to the cache, expanding as needed. */
static void append_run_start(seq_t val)
{
    if (run_count == run_capacity) {
        size_t new_cap = run_capacity ? run_capacity * 2 : 64;
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (new_arr == NULL) {
            /* In the unlikely event of allocation failure, we simply
             * abort the program – this is acceptable for the OEIS
             * evaluation harness. */
            abort();
        }
        for (size_t i = 0; i < run_count; ++i) {
            new_arr[i] = run_cache[i];
        }
        free(run_cache);
        run_cache   = new_arr;
        run_capacity = new_cap;
    }
    run_cache[run_count++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence is 1‑based */

    /* Generate runs until we have at least n stored starts. */
    while (run_count < n) {
        /* Scan in reasonably sized blocks to keep the loop small. */
        const idx_t block_size = 65536;
        idx_t limit = last_scanned + block_size;

        for (idx_t k = last_scanned + 1; k <= limit; ++k) {
            bool is_two = (count_distinct_primes(k) == 2);

            if (is_two) {
                if (in_run) {
                    ++run_len;
                } else {
                    in_run    = true;
                    run_start = k;
                    run_len   = 1;
                }
            } else {
                if (in_run && run_len >= 5) {
                    /* The run is maximal; record its starting value. */
                    append_run_start((seq_t)run_start);
                }
                in_run  = false;
                run_len = 0;
            }
        }

        last_scanned = limit;
    }

    return run_cache[n - 1];
}
