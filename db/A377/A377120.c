/*
 * A377120  a(n) = number of iterations of x -> 2 x + 3 to reach a nonprime,
 *          starting with prime(n) .
 * https://oeis.org/A377120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 5575, u32 n <= 5575, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS definition:
   a(n) = number of iterations of x -> 2*x + 3 until a nonprime is
   reached, starting with the n-th prime (offset 1).  It uses the
   provided helpers get_prime(idx_t) to obtain the prime and
   is_prime_big(seq_t) to test primality on arbitrary‑size integers. */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1; return 0 for safety */
        return 0;

    seq_t x   = (seq_t)get_prime(n);   /* start value = prime(n) */
    seq_t cnt = 0;                     /* count of iterations */

    while (is_prime_big(x)) {
        x   = x * (seq_t)2 + (seq_t)3;
        ++cnt;
    }
    return cnt;
}
