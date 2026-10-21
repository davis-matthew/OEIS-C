/*
 * A247270  Let k == 1 or 5 (mod 6) (A007310). a(n) is the greatest number of
 *          the initial values of k such that k^2+6*n-4 divided by the maximal
 *          possible power of 3 takes only prime values or 1.
 * https://oeis.org/A247270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) is the largest t such that for the first t numbers
 * k ≡ 1 or 5 (mod 6), the integer (k^2 + 6*n − 4) divided by all
 * powers of 3 is either prime or 1. The function tests the k’s
 * in order until this condition fails and returns the count t.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t k = 1;          /* first k ≡ 1 (mod 6) */
    bool step4 = true;    /* next step is +4 to reach 5 (mod 6) */
    idx_t count = 0;

    for (;;)
    {
        /* Compute k^2 + 6*n - 4 in seq_t */
        seq_t val = (seq_t)k * (seq_t)k + (seq_t)6 * (seq_t)n - 4;

        /* Remove all factors of 3 */
        while ((val % 3) == 0)
            val /= 3;

        /* Accept 1 or a prime */
        if (val == 1 || is_prime_big(val))
        {
            ++count;                      /* this k is good */
            k += step4 ? 4 : 2;           /* advance to next k */
            step4 = !step4;               /* alternate +4, +2 */
        }
        else
            break;                        /* first failure */
    }

    return (seq_t)count;
}
