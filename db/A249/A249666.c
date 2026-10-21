/*
 * A249666  Numbers n such that the sum of n and the largest prime<n
 *          (A151799(n)) is prime.
 * https://oeis.org/A249666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers n such that n + A151799(n) is prime
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    seq_t prevPrime = 2;      /* largest prime < current m */
    idx_t  count = 0;         /* how many terms found so far */

    for (idx_t m = 3; ; ++m)  /* iterate over integers starting from 3 */
    {
        seq_t sum = (seq_t)m + prevPrime;
        if (isPrimeTrial(sum))
        {
            ++count;
            if (count == n)
                return (seq_t)m;
        }

        if (isPrimeTrial((seq_t)m))
            prevPrime = (seq_t)m;   /* update largest prime for next m */
    }
}
