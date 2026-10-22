/*
 * A347247  Numbers k such that the greatest prime factor of A000593(k) [the
 *          sum of their odd divisors] is at least as large as the greatest
 *          prime factor of k itself.
 * https://oeis.org/A347247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that * GPF( A000593(k) ) ≥ GPF( k
 *            ), * where A000593(k) is the sum of odd divisors of k. The
 *            sequence * is generated locally by scanning integers k ≥ 1 and
 *            keeping the ones * that satisfy the condition, storing them in
 *            sorted order. The result * is memoised, so call...
 * Range:     u8 n <= 35, u16 n <= 1768, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, largest_prime_factor, odd_part, sigma_of, to_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>        /* for memset */
#include <limits.h>        /* for UINT64_MAX */

/* Include the required utilities for prime factors, sigma,
 * odd part, and array growth. */
#include "../utils/largest_prime_factor.h"   /* uint64_t largest_prime_factor(uint64_t); */
#include "../utils/odd_part.h"                /* seq_t odd_part(seq_t); */
#include "../utils/sigma_of.h"               /* seq_t sigma_of(idx_t); */
#include "../utils/grow_seq.h"               /* void grow_seq(seq_t **arr, size_t *cap, size_t need); */
#include "../utils/to_idx.h"

/* Internal state for memoisation */
static seq_t *terms = NULL;      /* array of found k's (as seq_t) */
static size_t terms_count = 0;   /* number of terms stored */
static size_t terms_capacity = 0;/* allocated size of terms array */
static idx_t last_checked = 0;   /* largest k that has been tested */

/* Helper to convert seq_t to idx_t safely */

/* Main function requested by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but safe fallback */

    /* Initialise storage on first call */
    if (terms == NULL) {
        terms_capacity = 64;
        terms = (seq_t*)calloc(terms_capacity, sizeof(seq_t));
        if (!terms) abort(); /* allocation failure */
    }

    /* Ensure we have enough terms */
    while (terms_count < (size_t)n) {
        /* Test next integer k */
        idx_t k = ++last_checked;   /* start from 1 on first iteration */

        /* Compute largest prime factor of k */
        uint64_t lpf_k = largest_prime_factor((uint64_t)k);

        /* Compute sum of odd divisors: sigma( odd_part(k) ) */
        seq_t odd = odd_part((seq_t)k);        /* odd_part returns seq_t */
        seq_t sum = sigma_of(to_idx(odd));     /* sigma_of expects idx_t */

        /* Convert sum to uint64_t for prime factorisation.
           Assume sum fits in 64 bits (true for all k examined here). */
        uint64_t sum_ull = (uint64_t)sum;

        uint64_t lpf_sum = largest_prime_factor(sum_ull);

        if (lpf_sum >= lpf_k) {
            /* Store k as seq_t */
            if (terms_count == terms_capacity) {
                terms_capacity *= 2;
                terms = (seq_t*)realloc(terms, terms_capacity * sizeof(seq_t));
                if (!terms) abort();
            }
            terms[terms_count++] = (seq_t)k;
        }
    }

    return terms[(size_t)n - 1];
}
