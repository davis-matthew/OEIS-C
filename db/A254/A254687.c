/*
 * A254687  Number of decompositions of 2n into sums of two primes p1 < p2
 *          such that p2-p1-1 is also a prime.
 * https://oeis.org/A254687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of decompositions of 2n into sums of two primes *
 *            p1 < p2 such that p2 − p1 − 1 is also a prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the definition by a simple sieve on the fly.  For each n
 * we build a sieve up to 2n, then count all prime pairs (p1,p2) with
 * p1 < p2, p1 + p2 = 2n, and p2 − p1 − 1 also prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    idx_t limit = 2 * n;                       /* we need primes up to 2n */
    /* Allocate sieve array */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return (seq_t)0;            /* out of memory fallback */

    /* Sieve of Eratosthenes */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= limit; m += p) {
                is_prime[m] = false;
            }
        }
    }

    seq_t count = 0;
    /* Check all prime pairs with p1 < p2 */
    idx_t half = limit / 2;
    for (idx_t p1 = 2; p1 <= half; ++p1) {
        if (!is_prime[p1]) continue;
        idx_t p2 = limit - p1;
        if (p2 <= p1) break;
        if (!is_prime[p2]) continue;
        idx_t diff = p2 - p1 - 1;
        if (diff <= limit && is_prime[diff]) {
            count++;               /* one valid decomposition found */
        }
    }
    free(is_prime);
    return count;
}
