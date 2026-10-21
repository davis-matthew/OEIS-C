/*
 * A253973  Primes p such that p + nextprime(p) is divisible by 9.
 * https://oeis.org/A253973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A253973 – primes p such that p + nextprime(p) is divisible by 9
 *            Computation: iterate primes using next_prime until the nth such
 *            prime is found
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached = 0;
    static size_t cap = 0;
    static uint64_t last_prime = 2;   /* start from the first prime 2 */

    if (n == 0) return 0;           /* undefined, but guard against 0 */

    /* Grow cache if it is empty */
    if (cached == 0 && cap == 0) {
        cap = 16;
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
    }

    /* Extend cache until we have at least n terms */
    uint64_t p = last_prime;
    while (cached < n) {
        uint64_t q = next_prime(p);          /* next prime after p          */
        if ((p + q) % 9 == 0) {              /* p + next_prime(p) divisible? */
            if (cached == cap) {              /* double capacity when needed */
                cap *= 2;
                cache = (seq_t*)realloc(cache, sizeof(seq_t) * cap);
            }
            cache[cached++] = (seq_t)p;      /* store the qualifying prime   */
        }
        p = q;                               /* advance to next candidate    */
    }

    last_prime = p;          /* last processed prime for future calls */

    return cache[n - 1];
}
