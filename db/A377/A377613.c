/*
 * A377613  a(n) is the number of iterations of x -> 2*x + 3 until (#
 *          composites reached) = (# primes reached), starting with prime(n).
 * https://oeis.org/A377613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 none, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A377613:  start with prime(n) and iterate  x → 2x+3
    until the number of primes seen equals the number of composites
    seen.  Return the count of iterations performed. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"    /* uint32_t get_prime(idx_t k);       */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n);       */

seq_t a(idx_t n)
{
    /* Starting prime */
    seq_t x = (seq_t)get_prime(n);

    /* Counts of primes and composites seen so far. */
    seq_t primes     = 1;   /* the starting prime itself */
    seq_t composites = 0;
    seq_t steps      = 0;   /* number of iterations performed */

    while (primes != composites) {
        x = x * ((seq_t)2) + (seq_t)3;

        if (is_prime_big(x))
            primes++;
        else
            composites++;

        steps++;
    }

    return steps;
}
