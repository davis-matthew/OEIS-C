/*
 * A320718  Indices of primes followed by a gap (distance to next larger
 *          prime) of 40.
 * https://oeis.org/A320718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes OEIS sequence A320718: indices i of primes such that
   prime(i+1) = prime(i) + 40.  It incrementally generates primes using
   the provided next_prime function and memoises results so that calls
   in any order are fast. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for n < 1 */

    /* static state shared between calls */
    static idx_t  count      = 0;   /* number of stored indices   */
    static idx_t  capacity   = 0;   /* allocated capacity        */
    static seq_t *cache      = NULL;/* array of found indices     */
    static uint64_t curr_p   = 2;   /* current prime p_i          */
    static idx_t   curr_i    = 1;   /* index of curr_p (1‑based) */

    /* Ensure the cache can hold the required number of terms. */
    if (n > capacity) {
        idx_t new_cap = (capacity == 0) ? 128 : capacity << 1;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* out of memory */
        cache    = tmp;
        capacity = new_cap;
    }

    /* Generate primes until we have at least n indices. */
    while (count < n) {
        uint64_t next = next_prime(curr_p);  /* next prime after curr_p */
        if (next - curr_p == 40) {
            cache[count++] = (seq_t)curr_i;   /* store current index i */
        }
        curr_p = next;
        curr_i++;
    }

    return cache[n - 1];
}
