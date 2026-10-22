/*
 * A377622  a(n) is the number of iterations of x -> 6*x - 5 until (#
 *          composites reached) = (# primes reached), starting with prime(n).
 * https://oeis.org/A377622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x -> 6*x - 5 until (# composites
 *            reached) = (# primes reached), starting with prime(n)
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    uint32_t p = get_prime(n);
    seq_t current = (seq_t)p;
    seq_t prime_cnt = 1;   /* starting prime */
    seq_t comp_cnt  = 0;
    seq_t steps     = 0;

    while (prime_cnt != comp_cnt)
    {
        current = current * (seq_t)6 - (seq_t)5;
        if (is_prime_big(current))
            prime_cnt++;
        else
            comp_cnt++;
        steps++;
    }
    return steps;
}
