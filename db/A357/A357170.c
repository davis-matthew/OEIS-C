/*
 * A357170  Primes p such that the minimum number of divisors among the
 *          numbers between p and NextPrime(p) is a prime power.
 * https://oeis.org/A357170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   count_distinct_primes, divisor_count_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A357170 – sequence of primes p such that the minimum divisor count
 * among the integers between p and the next prime after p is a prime power */

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/count_distinct_primes.h"

/* Return the n-th element of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for the offset requested, but harmless */
        return (seq_t)0;

    seq_t  result = 0;          /* value to return when we find the n‑th term */
    uint64_t p = 2;             /* current prime to examine */
    size_t found = 0;           /* number of valid terms discovered so far */

    while (found < n) {
        uint64_t next = next_prime(p);          /* prime immediately after p */

        /* Consider the interval (p, next).  Only meaningful if there is a gap. */
        if (p > 2 && next > p + 1) {
            uint64_t min_tau = UINT64_MAX;

            for (uint64_t k = p + 1; k < next; ++k) {
                uint64_t tau = divisor_count_u64(k);
                if (tau < min_tau)
                    min_tau = tau;
            }

            /* min_tau is a prime power iff it has exactly one distinct prime factor */
            if (count_distinct_primes(min_tau) == 1) {
                ++found;
                if (found == n)
                    result = (seq_t)p;
            }
        }

        p = next;           /* advance to the next prime */
    }

    return result;
}
