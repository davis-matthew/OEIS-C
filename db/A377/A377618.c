/*
 * A377618  a(n) is the number of iterations of x -> 4*x - 1 until (#
 *          composites reached) = (# primes reached), starting with prime(n).
 * https://oeis.org/A377618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: a(n) = number of iterations of the map x → 4*x − 1
   until the count of primes equals the count of composites,
   starting from the nth prime. */
#include <stdbool.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t k) -> uint32_t prime */
#include "../utils/is_prime_big.h"/* is_prime_big(seq_t n) -> bool */

seq_t a(idx_t n)
{
    /* Starting value: the nth prime (1‑indexed) */
    seq_t x = (seq_t)get_prime(n);
    seq_t primes = 1;      /* starting prime counts as first prime */
    seq_t composites = 0;  /* no composites yet */
    seq_t iter = 0;        /* iterations performed */

    while (primes != composites)
    {
        x = 4 * x - 1;
        iter++;

        if (is_prime_big(x))
            primes++;
        else
            composites++;
    }

    return iter;
}
