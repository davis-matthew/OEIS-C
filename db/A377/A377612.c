/*
 * A377612  a(n) is the number of iterations of x -> 2*x + 1 until (#
 *          composites reached) = (# primes reached), starting with prime(n).
 * https://oeis.org/A377612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 23, u32 n <= 23, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A377612
 *
 * a(n) = number of iterations of the map x → 2*x + 1 starting from the nth prime
 *        until the count of primes reached equals the count of composites reached.
 *        The starting prime is counted in the prime counter but does not contribute to
 *        the iteration count.
 */

#include <stdbool.h>                 /* for bool */
#include "../utils/get_prime.h"      /* get_prime(idx_t) : returns uint32_t */
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) : primality test for seq_t */

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n < 1; return 0 as safe fallback */
        return 0;

    seq_t x = (seq_t)get_prime(n);   /* start value: the nth prime */
    idx_t prime_cnt = 1;             /* the starting prime itself */
    idx_t comp_cnt  = 0;
    idx_t steps     = 0;

    while (prime_cnt != comp_cnt) {
        x = x * 2 + 1;               /* next term of the chain */
        steps++;
        if (is_prime_big(x))
            ++prime_cnt;
        else
            ++comp_cnt;
    }

    return (seq_t)steps;            /* number of iterations performed */
}
