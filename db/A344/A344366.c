/*
 * A344366  Integers k such that the sum of squares of digits of both k and
 *          k-1 are prime.
 * https://oeis.org/A344366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   digit_square_sum, is_prime_big, seq_alloc_new_array,
 *            seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function searches incrementally for integers k where the sum of
   squares of digits of k and of k‑1 are both prime.  It memoizes the
   results in a static dynamic array so that any index can be requested
   in any order. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_square_sum.h"   /* uint64_t digit_square_sum(uint64_t) */
#include "../utils/is_prime_big.h"       /* bool is_prime_big(seq_t) */
#include "../utils/seq_alloc_new_array.h" /* seq_t *seq_alloc_new_array(size_t) */
#include "../utils/seq_free_array.h"       /* void seq_free_array(seq_t *) */

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term.  Below that the result is
       irrelevant, but a small guard is harmless. */
    if (n < 1)
        return 0;

    /* Static state for memoisation */
    static seq_t *cache   = NULL; /* array of found terms */
    static size_t cap     = 0;    /* current capacity of the array */
    static size_t len     = 0;    /* number of terms stored */
    static uint64_t next_candidate = 1; /* smallest integer to test next */

    /* Generate terms until we have at least n of them. */
    while (len < n)
    {
        /* Search for the next qualifying k.  We start with the first
           integer strictly larger than the last found candidate. */
        uint64_t k;
        for (k = next_candidate + 1; ; ++k)
        {
            /* Sum of squares of digits for k. */
            uint64_t sum_k = digit_square_sum(k);
            if (!is_prime_big((seq_t)sum_k))
                continue;

            /* Sum of squares of digits for k-1. */
            uint64_t sum_km1 = digit_square_sum(k - 1);
            if (!is_prime_big((seq_t)sum_km1))
                continue;

            /* k satisfies the condition. */
            next_candidate = k;
            break;
        }

        /* Ensure we have enough capacity to store the new term. */
        if (len == cap)
        {
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = seq_alloc_new_array(new_cap);
            if (!tmp)
                exit(EXIT_FAILURE);   /* allocation failure */

            /* Copy existing terms to the new array. */
            for (size_t i = 0; i < len; ++i)
                tmp[i] = cache[i];

            /* Free old storage and replace with the new array. */
            if (cache)
                seq_free_array(cache);
            cache = tmp;
            cap   = new_cap;
        }

        /* Store the newly found term. */
        cache[len++] = (seq_t)next_candidate;
    }

    /* Return the requested element. */
    return cache[n - 1];
}
