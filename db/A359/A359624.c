/*
 * A359624  Centered triangular numbers which are products of three distinct
 *          primes.
 * https://oeis.org/A359624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 33, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   count_distinct_primes, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A359624: centered triangular numbers that are products of three
   distinct primes.  The function finds the n‑th such number by iterating
   through centered triangular numbers, testing each for squarefreeness
   and exactly three distinct prime factors. */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/count_distinct_primes.h"

/* Helper to test whether a value is a product of exactly three distinct
   primes.  It uses the fast 64‑bit routine when the value fits in a
   uint64_t; otherwise a simple trial division is used. */
static bool is_product_of_three_distinct_primes(seq_t v)
{
    /* squarefree is a prerequisite */
    if (!is_squarefree(v))
        return false;

    /* 64‑bit case – much faster */
    if (v <= UINT64_MAX) {
        uint64_t n = (uint64_t)v;
        return (count_distinct_primes(n) == 3);
    }

    /* Big‑integer case – naive trial division, enough for the sizes
       needed by the tests. */
    seq_t val = v;
    int count = 0;

    /* factor 2 if present */
    if ((val & 1) == 0) {
        count++;
        val >>= 1;                /* remove the factor 2 */
        /* because v is squarefree there is no further 2 */
    }

    /* iterate over odd integers */
    for (seq_t d = 3; d * d <= val && count <= 3; d += 2) {
        if (val % d == 0) {
            count++;
            val /= d;              /* remove the factor d once */
            /* squarefree guarantees it does not occur again */
        }
    }

    if (val > 1)                    /* remaining prime factor */
        count++;

    return (count == 3);
}

/* Main sequence function.
   It memoises the terms so that calls in any order are efficient. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_m = 1;   /* last m that was processed */

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        last_m++;
        seq_t m = (seq_t)last_m;
        seq_t tri = ((seq_t)3 * m * (m - 1)) / 2 + 1;

        if (is_product_of_three_distinct_primes(tri)) {
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = tri;
        }
    }

    return cache[n - 1];
}
