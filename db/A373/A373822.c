/*
 * A373822  Sum of the n-th maximal run of first differences of odd primes.
 * https://oeis.org/A373822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Run‑sum of maximal runs of consecutive equal gaps between odd primes. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;      /* cached run sums */
static size_t cache_len = 0;      /* number of cached sums */
static size_t cache_cap = 0;      /* capacity of the cache */

static uint64_t prev_prime = 3;   /* last odd prime processed */
static int64_t prev_gap = -1;     /* previous gap value (-1 means undefined) */
static seq_t run_sum = 0;         /* sum of the current run */

/* Ensure the cache can hold at least 'needed' entries. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    if (new_cap < needed)
        new_cap = needed;
    void *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    cache = (seq_t*)tmp;
    cache_cap = new_cap;
}

/* Generate run sums until we have at least 'target' terms cached. */
static void generate_runs_to(idx_t target)
{
    while (cache_len < (size_t)target) {
        /* Find next odd prime. */
        uint64_t cand = prev_prime + 2;
        while (!is_prime((idx_t)cand))
            cand += 2;
        uint64_t curr_prime = cand;
        uint64_t gap = curr_prime - prev_prime;

        if (prev_gap == -1) {
            /* First run starts. */
            run_sum = (seq_t)gap;
            prev_gap = (int64_t)gap;
        } else if (gap == (uint64_t)prev_gap) {
            /* Extend current run. */
            run_sum += (seq_t)gap;
        } else {
            /* Run ends: store previous run sum. */
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = run_sum;
            /* Start new run. */
            run_sum = (seq_t)gap;
            prev_gap = (int64_t)gap;
        }
        prev_prime = curr_prime;
    }
}

/* Public function, as required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if ((size_t)n > cache_len)
        generate_runs_to(n);
    return cache[n - 1];
}
