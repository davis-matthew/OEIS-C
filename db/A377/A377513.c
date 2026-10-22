/*
 * A377513  a(n) = number of iterations of x -> 2 x - 5 to reach a nonprime,
 *          starting with prime(n+3).
 * https://oeis.org/A377513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x -> 2*x-5 to reach a non‑prime,
 *            starting from prime(n+3). The sequence is defined for n >= 1
 * Range:     u8 n <= 1, u16 n <= 4121, u32 n <= 5780, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"      /* get_prime(idx_t k) → uint32_t */
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t n) -> bool */

seq_t a(idx_t n)
{
    /* start at the prime indexed n+3 (offset 1: a(1) starts at prime(4)) */
    seq_t x = (seq_t)get_prime(n + 3);
    seq_t steps = 0;

    /* iterate until x is not prime */
    while (is_prime_big(x)) {
        x = x * 2 - 5;
        ++steps;
    }

    return steps;
}
