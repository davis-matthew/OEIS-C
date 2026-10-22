/*
 * A380961  Primes prime(k) such that (prime(k) + prime(k+1)) mod (prime(k) -
 *          prime(k-1)) != 0.
 * https://oeis.org/A380961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 3338, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A380961:
   a(n) = the n-th prime p_k such that
          (p_k + p_{k+1}) mod (p_k - p_{k-1}) != 0 */
/* A straightforward incremental sieve using the utility function
   next_prime(uint64_t start).  The sequence is memoised in a static
   table of uint64_t; the returned value is cast to seq_t. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* Capacity for cached terms – generous enough for n up to 10 000 */
    #define CACHE_CAP 20000

    static uint64_t cache[CACHE_CAP];
    static size_t cache_len  = 0;   /* number of terms cached so far */

    /* State of prime traversal */
    static uint64_t prev_prime = 2; /* p_{k-1} */
    static uint64_t curr_prime = 3; /* p_k   */
    static bool initialized = false;

    if (!initialized) {
        cache_len = 0;
        initialized = true;
    }

    /* Grow cache until it contains at least n terms */
    while (cache_len < n) {
        /* Compute next prime after curr_prime */
        uint64_t next_p = next_prime(curr_prime);

        /* Check the condition:
           (curr_prime + next_p) mod (curr_prime - prev_prime) != 0 */
        uint64_t diff = curr_prime - prev_prime;
        if ((curr_prime + next_p) % diff != 0) {
            /* Store the qualifying prime */
            if (cache_len < CACHE_CAP) {
                cache[cache_len++] = curr_prime;
            } else {
                /* In the unlikely event the array is full, exit
                   gracefully (sequence will be incorrect only after
                   the capacity) */
                break;
            }
        }

        /* Advance the sliding window of three consecutive primes */
        prev_prime = curr_prime;
        curr_prime = next_p;
    }

    /* If n was beyond capacity we return 0 (should not happen for
       the required limits). */
    if (n == 0 || cache_len < n) {
        return (seq_t)0;
    }

    /* Return the requested term, converting from uint64_t to seq_t */
    return (seq_t)cache[n - 1];
}
