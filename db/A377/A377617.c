/*
 * A377617  a(n) is the number of iterations of x -> 3*x + 4 until (#
 *          composites reached) = (# primes reached), starting with prime(n).
 * https://oeis.org/A377617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the number of iterations of x -> 3*x + 4 until the number of
    primes encountered equals the number of composites encountered.
    The chain starts at the n‑th prime, p(n).  This implementation
    simply performs the iteration until the counts balance. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"      /* provides get_prime(idx_t) */
#include "../utils/is_prime_big.h"   /* provides is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* Start with the n-th prime */
    uint32_t p = get_prime(n);
    seq_t x = (seq_t)p;

    /* Counters for primes and composites (prime counts starts at 1) */
    size_t primes_cnt = 1;
    size_t composites_cnt = 0;

    /* Count of iterations performed */
    idx_t steps = 0;

    /* Iterate until the counts are equal */
    for (;;) {
        x = x * 3 + 4;   /* apply the Map */
        steps++;

        if (is_prime_big(x))
            ++primes_cnt;
        else
            ++composites_cnt;

        if (primes_cnt == composites_cnt)
            break;
    }

    return (seq_t)steps;
}
