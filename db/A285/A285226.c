/*
 * A285226  Primes with integer arithmetic mean of digits = 5 in base 10.
 * https://oeis.org/A285226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Primes p such that digit_sum(p) == 5 * digit_len(p) (average
 *            digit = 5)
 * Range:     u8 n <= 4, u16 n <= 557, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, digit_len, digit_sum, ensure_capacity,
 *            isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"   /* for isPrimeGeneric */
#include "../utils/digit_sum.h"        /* for digit_sum */
#include "../utils/digit_len.h"        /* for digit_len */
#include "../utils/append_candidate.h" /* for append_candidate */
#include "../utils/ensure_capacity.h"  /* for ensure_capacity */

/* Return the n-th prime whose integer arithmetic mean of digits equals 5.
 * The sequence is indexed from 1. */
seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *cache = NULL;      /* array of found primes */
    static size_t cache_len = 0;     /* number of elements stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    /* Static variable to remember how far we have searched */
    static seq_t current = 2;        /* next integer to test */

    if (n == 0)  /* offset is 1; for n<1 behavior is irrelevant */
        return 0;

    /* Grow cache capacity as needed */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Keep generating primes until we have at least n terms */
    while (cache_len < (size_t)n) {
        if (isPrimeGeneric(current)) {
            seq_t sum   = digit_sum(current);
            int  digits = digit_len(current);
            if (sum == 5 * (seq_t)digits) {
                /* Append to cache */
                append_candidate(&cache, &cache_len, &cache_cap, current);
            }
        }
        current++;  /* move to next candidate */
    }

    return cache[n - 1];
}
