/*
 * A253610  Numbers n with property that the sum of n and the digital root of
 *          n is prime.
 * https://oeis.org/A253610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A253610: numbers n such that n + digital_root(n) is prime. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* ----- Cached state ----- */
static seq_t *cache = NULL;      /* found terms in order */
static size_t cache_len = 0;     /* how many terms are stored */
static size_t cache_cap = 0;     /* capacity of the cache array */
static idx_t  last_tested = 0;   /* largest integer already examined */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In a production environment proper error handling would be used. */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* A positive integer n satisfies the property iff */
/* n + digital_root(n) is prime.  This function tests that. */
static bool is_valid(idx_t n)
{
    idx_t dr = n % 9;
    if (dr == 0)
        dr = 9;
    idx_t sum = n + dr;
    return is_prime(sum);  /* prime test on idx_t (wide) avoids overflow */
}

/* Main entry for the sequence.  Returns the n-th element for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* undefined for index zero */

    /* Return from cache if already computed. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Generate new terms until the required index is reached. */
    for (;;) {
        idx_t candidate = last_tested + 1;
        last_tested = candidate;
        if (is_valid(candidate)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)candidate;   /* cast to seq_t for storage */
            if ((size_t)n == cache_len)
                return (seq_t)candidate;
        }
    }
}
