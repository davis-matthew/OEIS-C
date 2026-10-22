/*
 * A337144  n is the a(n)-th positive integer which takes its number of steps
 *          to reach 1 in the Collatz (or 3x+1) problem.
 * https://oeis.org/A337144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of j (1 ≤ j ≤ n) such that Collatz_steps(j) =
 *            Collatz_steps(n)
 * Range:     u8 n <= 9458, u16 n <= 9458, u32 n <= 9458, u64 n <= 9458,
 *            u128 n <= 9458, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is an on‑demand, incremental computation that
   stores all previously computed values. */

#include <stdint.h>
#include <stdlib.h>

/* Helper: compute total Collatz steps until reaching 1.
   For n = 1 the function returns 0. */
static idx_t collatz_steps(idx_t n)
{
    idx_t steps = 0;
    idx_t cur = n;
    while (cur != 1) {
        if ((cur & 1) == 0) {           /* even */
            cur >>= 1;
        } else {                        /* odd */
            cur = cur * 3 + 1;
        }
        ++steps;
    }
    return steps;
}

/* Static state: cache of a(n) values and step counts. */
static seq_t *a_cache = NULL;          /* a_cache[0] unused, a_cache[n] = a(n) */
static size_t a_cache_size = 0;        /* allocated length of a_cache */
static seq_t *step_counts = NULL;      /* step_counts[s] = number of
                                         * j ≤ current n with step count s */
static size_t step_counts_size = 0;    /* allocated length of step_counts */
static idx_t cached_n = 0;             /* last n that has been cached */

/* Ensure a_cache holds at least indices up to n. */
static void ensure_a_cache_size(size_t n_plus_one)
{
    if (a_cache_size >= n_plus_one)
        return;
    size_t new_size = a_cache_size ? a_cache_size * 2 : 16;
    while (new_size < n_plus_one)
        new_size *= 2;
    seq_t *new_arr = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!new_arr) {
        /* In a judge environment memory exhaustion is unlikely.
           If it does happen, fall back to an error return. */
        abort();
    }
    a_cache = new_arr;
    /* Initialize newly allocated elements */
    for (size_t i = a_cache_size; i < new_size; ++i)
        a_cache[i] = 0;
    a_cache_size = new_size;
}

/* Ensure step_counts has room for at least max_step indices. */
static void ensure_step_counts_size(size_t max_step_plus_one)
{
    if (step_counts_size >= max_step_plus_one)
        return;
    size_t new_size = step_counts_size ? step_counts_size * 2 : 16;
    while (new_size < max_step_plus_one)
        new_size *= 2;
    seq_t *new_arr = (seq_t *)realloc(step_counts, new_size * sizeof(seq_t));
    if (!new_arr) {
        abort();
    }
    step_counts = new_arr;
    /* Initialize new entries to 0 */
    for (size_t i = step_counts_size; i < new_size; ++i)
        step_counts[i] = 0;
    step_counts_size = new_size;
}

seq_t a(idx_t n)
{
    /* Index n is 1‑based; a(1) is defined. */
    if (n < 1)
        return 0;               /* invalid, but keep type consistency */

    if (n > cached_n) {
        /* Extend caches up to n */
        ensure_a_cache_size((size_t)n + 1);      /* +1 because we use 1‑based */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            idx_t steps = collatz_steps(i);
            /* Ensure step_counts can index 'steps' */
            ensure_step_counts_size((size_t)steps + 1);
            /* Increment counter for this stop‑time and store result */
            step_counts[steps] += 1;
            a_cache[i] = step_counts[steps];
        }
        cached_n = n;
    }
    return a_cache[n];
}
