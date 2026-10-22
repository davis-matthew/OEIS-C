/*
 * A377610  a(n) is the number of iterations of x -> 2*x - 3 until (#
 *          composites reached) = (# primes reached), starting with
 *          prime(n+2).
 * https://oeis.org/A377610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 3, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate x -> 2*x-3 until the number of primes equals the number of composites
   starting with prime(n+2). Return the number of iterations performed.
   The function uses get_prime(k) to fetch the k‑th prime and
   is_prime_big(x) for primality testing. */

#include <stdbool.h>
#include "../utils/get_prime.h"   /* returns uint32_t get_prime(idx_t k) */
#include "../utils/is_prime_big.h"/* returns bool is_prime_big(seq_t n) */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)get_prime(n + 2);   /* starting prime */
    idx_t primes     = 1;                /* count of primes seen (include start) */
    idx_t composites = 0;                /* count of composites seen */
    idx_t steps      = 0;                /* number of iterations performed */

    while (primes != composites) {
        x = x + x - 3;                   /* next term: 2*x – 3 */
        ++steps;
        if (is_prime_big(x))
            ++primes;
        else
            ++composites;
    }

    return (seq_t)steps;
}
