/*
 * A262520  a(n) = A262519(n) - A262518(n).
 * https://oeis.org/A262520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A262519(n) - A262518(n)
 * Range:     i8 n <= 1919, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Static cache that holds divisor counts up to the largest value needed. */
static uint32_t *div_cache = NULL;
static idx_t div_cache_size = 0;

/* Make sure div_cache[k] is defined for all k <= limit. */
static void ensure_div_cache(idx_t limit)
{
    if (limit <= div_cache_size)
        return;

    /* Allocate new cache (cast for C++). */
    uint32_t *new_cache = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    if (!new_cache)
        exit(1);  /* abort on allocation failure */

    /* Sieve to count divisors for all numbers up to limit. */
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i)
            ++new_cache[(size_t)j];
    }

    /* Free old cache and replace it. */
    free(div_cache);
    div_cache = new_cache;
    div_cache_size = limit;
}

/* How many steps are needed to reach 0 when starting from k using k -> k - d(k). */
static seq_t steps_to_zero(idx_t k)
{
    seq_t steps = 0;
    while (k > 0) {
        /* div_cache is indexed by size_t; cast k accordingly. */
        idx_t d = (idx_t)div_cache[(size_t)k];
        k -= d;
        ++steps;
    }
    return steps;
}

seq_t a(idx_t n)
{
    /* Both 2*n and 2*n+1 are needed. */
    idx_t start   = 2 * n;
    idx_t max_k   = start + 1;        /* (2*n+1) */
    ensure_div_cache(max_k);

    seq_t steps_even = steps_to_zero(start);
    seq_t steps_odd  = steps_to_zero(start + 1);
    return steps_odd - steps_even;
}
