/*
 * A290200  Prime numbers of the form 4^k + 1.
 * https://oeis.org/A290200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   is_prime_big, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime of the form 4^k + 1 (k ≥ 0), offset 1. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* basic guard: sequence is defined for n ≥ 1 */
    if(n < 1) return 0;

    seq_t pow = 1;          /* current value of 4^k */
    idx_t  count = 0;       /* number of primes seen so far */

    for(idx_t k = 0;; ++k)
    {
        /* val = 4^k + 1 */
        seq_t val = pow;
        val += (seq_t)1;

        if(is_prime_big(val))
        {
            ++count;
            if(count == n)
                return val;
        }

        /* advance pow to 4^(k+1) */
        pow = mul_small(pow, 4);
    }
}
