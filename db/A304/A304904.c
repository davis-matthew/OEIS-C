/*
 * A304904  Greatest prime p such that 2*n^2 - p is prime.
 * https://oeis.org/A304904
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest prime p such that 2*n^2 - p is prime
 * Range:     u8 n <= 11, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"  /* assumes is_prime_big(seq_t) is declared here */

seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* undefined for n<2, guard just in case */

    /* compute 2*n^2 as seq_t */
    seq_t nn = (seq_t)n;
    seq_t target = (seq_t)2 * nn * nn;

    /* iterate over odd numbers below target, starting from target-1 */
    for (seq_t p = target - 1; p >= (seq_t)3; p -= (seq_t)2) {
        if (is_prime_big(p) && is_prime_big(target - p))
            return p;
    }
    /* fallback for very small n where only p=2 works (not the case here) */
    return (seq_t)2;
}
