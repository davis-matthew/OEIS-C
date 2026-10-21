/*
 * A248596  Smallest prime number R such that there is a prime number Q with
 *          floor(Q/R) = prime(n).
 * https://oeis.org/A248596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"

/* The smallest prime R such that there exists a prime Q with
 * floor(Q/R) = prime(n).  For a given n, let p = prime(n).
 * We seek the least prime R for which a prime Q lies in
 * [p*R , (p+1)*R – 1].  This is found by iterating over the
 * primes R and, for each, testing whether the first prime
 * ≥ low = p*R does not exceed high = (p+1)*R–1.  */

seq_t a(idx_t n)
{
    /* Retrieve the n‑th prime: p = prime(n). */
    uint32_t p32 = get_prime(n);
    idx_t p = (idx_t)p32;          /* p fits in idx_t for all n used. */

    /* Begin searching with the first prime R = 2. */
    uint64_t R = 2;
    for (;;) {
        /* Bounds of the interval in which Q must lie. */
        idx_t low  = p * R;
        idx_t high = (p + 1) * R - 1;

        /* Smallest prime Q ≥ low.  If it is ≤ high, R is valid. */
        uint64_t Q = next_prime(low - 1);
        if (Q <= high)
            return (seq_t)R;

        /* Otherwise go to the next prime R. */
        R = next_prime(R);
    }
}
