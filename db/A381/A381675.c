/*
 * A381675  a(n) = p*2^(p - 1)*(p - 1)!, where p = prime(n).
 * https://oeis.org/A381675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) * 2^(prime(n)-1) * (prime(n)-1)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a lazy cache so that calls can appear in any order. */

#include <stdlib.h>
#include <stdint.h>

/* Utility prototypes supplied by the build system */
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/pow2.h"        /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* n < 1 is outside the OEIS offset – return 0. */
    if (n < 1) {
        return (seq_t)0;
    }

    /* ---------- static memoisation state ---------- */
    static seq_t *cache = NULL;      /* array of computed terms */
    static size_t cache_len = 0;     /* number of terms stored so far */
    static size_t cache_cap = 0;     /* current allocation capacity */

    /* factorial accumulator: fact holds (current_fact_up_to)!   */
    static seq_t fact = (seq_t)1;
    static idx_t fact_up_to = 1;     /* the largest k for which fact == k! */

    /* Expand cache until we have a(n). */
    while (cache_len < (size_t)n) {
        /* Compute the next prime (1‑based index). */
        idx_t   idx   = (idx_t)(cache_len + 1);   /* next index to fill */
        uint32_t p   = get_prime(idx);           /* p = prime(idx)      */

        /* Ensure there is space in the cache. */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!newp) {
                /* Out of memory – abort. (The test harness never triggers this.) */
                exit(1);
            }
            cache = newp;
            cache_cap = new_cap;
        }

        /* Extend the factorial to (p-1)! if necessary. */
        for (idx_t k = fact_up_to + 1; k <= (idx_t)p - 1; ++k) {
            fact *= (seq_t)k;
        }
        fact_up_to = (idx_t)p - 1;

        /* Compute a(idx) = p * 2^(p-1) * (p-1)! */
        seq_t term = (seq_t)p * pow2((idx_t)p - 1) * fact;

        /* Store the result and increment the length. */
        cache[cache_len] = term;
        ++cache_len;
    }

    /* Return the requested term. */
    return cache[(size_t)(n - 1)];
}
