/*
 * A276230  a(n) is the A106490 index where n first occurs.
 * https://oeis.org/A276230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   int_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the product of the first n elements of the sequence
   2,2,3,3,4,5,5,7,7,9,11,11,13,13,17,17,19,19,23,23,25,29,29,...
   where each prime p occurs twice and each square of a prime p² occurs once.
   This gives the index of n in the A106490 sequence. */

#include "../utils/is_prime.h"
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* product of zero multipliers */

    seq_t prod = (seq_t)1;
    idx_t  count = 0;

    for (uint64_t num = 2; count < n; ++num)
    {
        if (is_prime(num))
        {
            prod *= (seq_t)num;
            ++count;
            if (count == n) return prod;

            prod *= (seq_t)num;   /* prime appears twice */
            ++count;
            if (count == n) return prod;
        }
        else
        {
            uint64_t r = int_sqrt(num);
            if (r * r == num && is_prime(r))
            {
                /* square of a prime appears once */
                prod *= (seq_t)num;
                ++count;
                if (count == n) return prod;
            }
        }
    }

    return prod;   /* should never reach here */
}
