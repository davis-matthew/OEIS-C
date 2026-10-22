/*
 * A381218  Odd numbers with both an odd number of prime factors (counted with
 *          multiplicity) and an odd number of distinct prime factors.
 *          (Intersection of A067019 and A098903.)
 * https://oeis.org/A381218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = odd numbers with both an odd number of prime factors
 *            (counted with multiplicity) and an odd number of distinct prime
 *            factors. The function returns the nth term of this sequence. It
 *            builds the list on demand, storing the results in a static
 *            dynamic array so that repeated calls, even in arbi...
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega, count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* static storage for generated values */
    static seq_t *seq_cache = NULL;
    static size_t seq_len   = 0;
    static size_t seq_cap   = 0;
    static idx_t  next_candidate  = 3;   /* first odd >1 */
    static bool   initialized    = false;

    if (!initialized) {
        /* start with a small capacity and allocate */
        seq_cap = 32;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len   = 0;
        next_candidate = 3;
        initialized = true;
    }

    /* convert n to a plain size_t for indexing */
    size_t target = (size_t)n;

    while (seq_len < target) {
        /* test the current odd number */
        if ((bigomega(next_candidate) & 1) &&
            (count_distinct_primes((uint64_t)next_candidate) & 1)) {

            /* grow cache if needed */
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        /* advance to next odd number */
        next_candidate += 2;
    }
    return seq_cache[target - 1];
}
