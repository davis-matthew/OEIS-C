/*
 * A383666  Numbers in whose binary representation no bit (0 or 1) occurs
 *          exactly once.
 * https://oeis.org/A383666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 220, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_zero_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term of OEIS A383666.
 * Sequence definition: numbers whose binary representation contains
 * neither the digit 0 nor the digit 1 exactly once.  (Equivalently,
 * exclude powers of two and numbers with exactly one zero in binary.)
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/count_zero_bits.h"

static seq_t *cache = NULL;      /* array of accumulated terms      */
static size_t cache_size = 0;    /* number of stored terms          */
static size_t cache_cap   = 0;    /* current capacity of 'cache'     */
static seq_t  last_tested = 2;    /* last integer examined (start 2) */

/* Ensure that 'cache' has at least 'need' slots available.  */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 256;
    if (new_cap < need)
        new_cap = need;

    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    if (!new_cache)
        abort();   /* allocation failure */

    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return true if x is a power of two.  */
static bool is_power_of_two(seq_t x)
{
    if (x == 0)
        return false;
    return (x & (x - 1)) == 0;
}

seq_t a(idx_t n)
{
    /* Ensure we have at least n terms cached. */
    while (cache_size < (size_t)n) {
        /* Generate next candidate integer.  */
        last_tested = last_tested + (seq_t)1;
        seq_t candidate = last_tested;

        /* Skip numbers < 3: only 3 qualifies among 0,…,2. */
        if (candidate < (seq_t)3)
            continue;

        /* Exclude powers of two (one 1 bit). */
        if (is_power_of_two(candidate))
            continue;

        /* Count zero bits.  Exclude numbers where there is exactly one zero. */
        if (count_zero_bits(candidate) == 1)
            continue;

        /* All conditions passed – store the candidate. */
        ensure_cache_capacity(cache_size + 1);
        cache[cache_size++] = candidate;
    }

    return cache[(size_t)n - 1];
}
