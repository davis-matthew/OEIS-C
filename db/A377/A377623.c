/*
 * A377623  a(n) is the number of iterations of x -> 6*x + 1 until (#
 *          composites reached) = (# primes reached), starting with prime(n).
 * https://oeis.org/A377623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A377623: a(n) = number of iterations of x → 6x+1 until the number of primes encountered equals the number of composites (starting from prime(n)). */
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t x        = (seq_t)get_prime(n);   /* starting prime prime(n) */
    seq_t primes  = 1;                      /* prime( n ) is already counted as a prime */
    seq_t composites = 0;
    seq_t iter    = 0;

    while (primes != composites) {
        x = x * 6 + 1;                     /* next member of the chain */
        if (is_prime_big(x))
            ++primes;
        else
            ++composites;
        ++iter;
    }
    return iter;                            /* number of iterations performed */
}
