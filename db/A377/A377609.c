/*
 * A377609  a(n) is the number of iterations of x -> 2*x - 1 until (#
 *          composites reached) = (# primes reached), starting with prime(n).
 * https://oeis.org/A377609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 21, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) is the number of iterations of x → 2*x-1
   starting with the n‑th prime until the numbers of primes and
   composites seen are equal.  The chain is started with prime(n);
   each iteration produces a new value, which is counted as prime or
   composite.  The function returns the number of arrows needed to
   reach equality. */
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k) */
#include "../utils/is_prime_big.h"/* bool is_prime_big(seq_t n) */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* start with the n-th prime (seq_t may be larger than 32 bits) */
    seq_t x = (seq_t)get_prime(n);

    seq_t primes = 1;      /* the starting value is prime           */
    seq_t composites = 0;  /* number of composites seen so far      */
    seq_t step = 0;        /* number of iterations performed       */

    while (primes != composites) {
        x = 2 * x - 1;      /* next element of the chain            */
        ++step;
        if (is_prime_big(x))
            ++primes;
        else
            ++composites;
    }

    return step;
}
