/*
 * A355485  Primes p such that neither g-1 nor g+1 is prime, where g is the
 *          gap from p to the next prime.
 * https://oeis.org/A355485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime p where neither (next_prime(p)-p)-1 * nor
 *            (next_prime(p)-p)+1 is prime. The function collects primes by *
 *            incremental generation and tests the condition. It memoises
 *            found * terms so that repeated calls with arbitrary n are
 *            efficient
 * Range:     u8 none, u16 n <= 76, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"
#include "../utils/next_prime.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache      = NULL;   /* array of qualifying primes */
    static size_t cap        = 0;      /* allocated capacity of cache   */
    static idx_t  count      = 0;      /* number of elements stored     */
    static uint64_t last_p   = 0;      /* last prime that was processed */

    if (count >= n)               /* we already have the requested term */
        return cache[n-1];

    /* grow the cache as needed */
    while (count < n) {
        uint64_t p = (last_p == 0) ? next_prime(0) : next_prime(last_p);
        last_p = p;

        uint64_t q = next_prime(p);
        uint64_t g = q - p;
        if (!is_prime_small((seq_t)(g-1)) && !is_prime_small((seq_t)(g+1))) {
            /* store qualifying prime */
            if (count+1 > cap) grow_seq(&cache, &cap, count+1);
            cache[count++] = (seq_t)p;
        }
    }
    return cache[n-1];
}
