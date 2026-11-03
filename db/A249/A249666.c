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
 * Validated: standard, pass, 77 terms through n = 802 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    seq_t prevPrime = 2;      /* largest prime < current m */
    idx_t  count = 0;         /* how many terms found so far */

    for (idx_t m = 3; ; ++m)  /* iterate over integers starting from 3 */
    {
        seq_t sum = (seq_t)m + prevPrime;
        if (is_prime_seq(sum))
        {
            ++count;
            if (count == n)
                return (seq_t)m;
        }

        if (is_prime_seq((seq_t)m))
            prevPrime = (seq_t)m;   /* update largest prime for next m */
    }
}
