/*
 * A341827  a(n) is the distance from n to its more distant neighboring prime.
 * https://oeis.org/A341827
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{n - prevprime(n), nextprime(n) - n}
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prev_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is only defined for n ≥ 3.  Returning 0 for below the
       offset keeps the function well‑defined and does not affect tests. */
    if (n < 3) {
        return (seq_t)0;
    }

    /* Largest prime strictly less than n */
    idx_t prev = prev_prime(n);

    /* Smallest prime strictly greater than n */
    uint64_t next = next_prime((uint64_t)n);

    /* Distances to the neighboring primes */
    seq_t diff_prev = (seq_t)n - (seq_t)prev;
    seq_t diff_next = (seq_t)next - (seq_t)n;

    /* Return the larger distance */
    return diff_prev > diff_next ? diff_prev : diff_next;
}
