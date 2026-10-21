/*
 * A246401  Smallest prime number Q such that the sum prime(n)+prime(n+1)+Q is
 *          a prime number.
 * https://oeis.org/A246401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 2830, u32 n <= 2831, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest prime Q such that prime(n)+prime(n+1)+Q is a prime number */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* fetch the n-th and (n+1)-th primes (1‑based indexing) */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);

    /* start with the first prime and search forward */
    uint64_t q = 2;
    for (;;) {
        seq_t sum = (seq_t)p1 + (seq_t)p2 + (seq_t)q;
        if (is_prime_big(sum))
            return (seq_t)q;

        /* move to the next prime after the current one */
        q = next_prime(q);
    }
}
