/*
 * A278744  Number of steps (modular reductions) in calculating the GCD of
 *          n-th consecutive primes p(n) and p(n+1) by the Euclidean
 *          algorithm.
 * https://oeis.org/A278744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of modular reductions in Euclid’s algorithm used
 *            to compute the GCD of the consecutive primes p(n) and p(n+1)
 * Range:     u8 n <= 53, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t k) returns the k-th prime (1‑based) */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, callers normally start at 1 */

    /* Retrieve the two consecutive primes: p(n) and p(n+1).  The function
       returns a 32‑bit unsigned integer; we cast it to seq_t for the
       arithmetic that follows. */
    seq_t b_val = (seq_t)get_prime(n);       /* p(n)   */
    seq_t a_val = (seq_t)get_prime(n + 1);   /* p(n+1) */

    seq_t steps = 0;
    seq_t r;

    /* Perform the Euclidean algorithm, counting modular reductions
       until the remainder becomes 1 (the GCD of two distinct primes). */
    while (true) {
        r = a_val % b_val;
        ++steps;
        if (r == (seq_t)1)
            break;
        a_val = b_val;
        b_val = r;
    }

    return steps;
}
