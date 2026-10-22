/*
 * A373828  Run-sums (differing by 0) of run-lengths (differing by 2) of odd
 *          primes.
 * https://oeis.org/A373828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of successive runs of equal run‑lengths of odd
 *            primes. The sequence is generated on demand
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Once a run finishes, its length is added to a group with equal lengths.
   When a run of a different length starts, the current group sum is stored
   as the next term of the sequence. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Memoised run‑sums cache */
static seq_t *run_sums = NULL;          /* array of computed values */
static size_t run_sums_len = 0;         /* number of stored values */
static size_t run_sums_cap = 0;         /* allocated capacity */

/* State for accumulating runs of odd primes */
static idx_t prev_prime = 0;            /* previous prime processed (0 if none) */
static uint64_t current_run_len = 0;    /* length of the current run of twin primes */

/* State for grouping equal run‑lengths */
static bool group_active = false;      /* true if a group is in progress */
static seq_t group_value = 0;           /* run‑length value for current group */
static seq_t group_total = 0;           /* sum of run‑lengths in current group */

/* Next odd integer to test for primality */
static idx_t candidate = 3;

/* Append a computed run‑sum value to the cache */
static void append_run_sum(seq_t val)
{
    if (run_sums_len == run_sums_cap) {
        run_sums_cap = run_sums_cap ? run_sums_cap * 2 : 4;
        run_sums = (seq_t *)realloc(run_sums, run_sums_cap * sizeof(seq_t));
    }
    run_sums[run_sums_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* OEIS offset starts at 1 */

    /* Allocate cache on first call */
    if (run_sums == NULL) {
        run_sums_cap = 4;
        run_sums = (seq_t *)malloc(run_sums_cap * sizeof(seq_t));
    }

    /* Generate primes and process runs until we have at least n terms */
    while (run_sums_len < n) {
        /* Find the next odd prime */
        while (!is_prime(candidate)) candidate += 2;
        uint64_t p = (uint64_t)candidate;

        if (prev_prime == 0) {
            /* First prime: start first run */
            current_run_len = 1;
        } else if (p - prev_prime == 2) {
            /* Twin‑prime: extend current run */
            current_run_len++;
        } else {
            /* Run finished: handle its length */
            seq_t L = (seq_t)current_run_len;

            if (!group_active) {
                /* First group */
                group_active = true;
                group_value = L;
                group_total = L;
            } else if (L == group_value) {
                /* Extend current group */
                group_total += L;
            } else {
                /* Different length: finalise previous group */
                append_run_sum(group_total);
                /* Start new group */
                group_value = L;
                group_total = L;
            }

            /* Start new run with current prime */
            current_run_len = 1;
        }

        /* Update state for next iteration */
        prev_prime = p;
        candidate = p + 2;
    }

    return run_sums[n - 1];
}
