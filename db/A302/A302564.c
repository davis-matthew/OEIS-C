/*
 * A302564  a(n) is the greatest prime p such that (2*n+1-p)/2 is prime.
 * https://oeis.org/A302564
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the greatest prime p such that (2*n+1-p)/2 is prime
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* For indices below the offset (n < 3), the value is undefined. */
    if (n < 3)
        return 0;

    /* Maximum candidate for p is 2*n - 3 (odd prime). */
    idx_t upper = 2 * n - 3;

    for (idx_t p = upper; p >= 3; p -= 2)        /* step by 2 to skip even numbers */
    {
        if (!is_prime_big((seq_t)p))
            continue;

        /* Compute q = (2*n + 1 - p) / 2. This is an integer because
           2*n + 1 and p are both odd, so their difference is even. */
        seq_t diff = ((seq_t)2 * (seq_t)n) + 1 - (seq_t)p;
        seq_t q = diff / 2;

        if (is_prime_big(q))
            return (seq_t)p;
    }

    /* If no such prime exists (theoretically impossible for n >= 3),
       return 0 as a sentinel. */
    return 0;
}
