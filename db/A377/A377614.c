/*
 * A377614  a(n) is the number of iterations of x -> 2*x + 5 until (#
 *          composites reached) = (# primes reached), starting with prime(n).
 * https://oeis.org/A377614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x → 2⋅x + 5 until the count of
 *            primes equals the count of composites, starting from the nth
 *            prime
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 3, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k) */
#include "../utils/is_prime_big.h"/* provides bool is_prime_big(seq_t n) */

seq_t a(idx_t n)
{
    /* Begin with the nth prime.  n is 1‑based. */
    uint32_t p = get_prime(n);
    seq_t x  = (seq_t)p;

    unsigned primes_seen   = 1; /* the starting prime is counted */
    unsigned composites_seen = 0;
    seq_t iterations = 0;

    /* Continue until the numbers of primes and composites are equal. */
    while (primes_seen != composites_seen) {
        x = x * 2 + 5;          /* next element of the chain */
        ++iterations;
        if (is_prime_big(x))
            ++primes_seen;
        else
            ++composites_seen;
    }

    return iterations;
}
