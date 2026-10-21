/*
 * A281993  Integers m such that sigma(m) + sigma(2*m) = 6*m.
 * https://oeis.org/A281993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A281993: Integers m such that sigma(m) + sigma(2*m) = 6*m.  
   Formula: a(n) = 2^k * (3*2^k − 1) where k is the n‑th index for which
   3*2^k − 1 is prime. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, undefined for n=0 */

    seq_t pow2 = (seq_t)2;   /* 2^1 for k = 1 */
    idx_t count = 0;

    for (idx_t k = 1; ; ++k)
    {
        seq_t p = pow2;
        p = p * (seq_t)3;   /* 3*2^k */
        p -= (seq_t)1;      /* 3*2^k − 1 */

        if (is_prime_big(p))
        {
            ++count;
            if (count == n)
            {
                seq_t result = pow2;
                result *= p;    /* 2^k * (3*2^k − 1) */
                return result;
            }
        }

        pow2 *= (seq_t)2;   /* advance to 2^(k+1) */
    }
}
