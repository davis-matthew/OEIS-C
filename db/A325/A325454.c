/*
 * A325454  a(n) is the digit sum of the n-th Niven number (or Harshad
 *          number).
 * https://oeis.org/A325454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 8678, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the digit sum of the nth Niven (Harshad) number by brute force.
   The nth Niven number is the nth integer that is divisible by the sum of its
   decimal digits. This implementation scans integers from 1 upward,
   checking the divisibility condition until the desired count is reached. */
#include <stdint.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1; value for n=0 is irrelevant */
    idx_t count = 0;
    uint64_t num = 1;          /* integer to test: always fits in 64-bit */
    seq_t result = 0;

    while (count < n)
    {
        seq_t dsum = digit_sum((seq_t)num);   /* digit sum of current number */
        if (dsum != 0 && ((seq_t)num % dsum) == 0)   /* Niven test */
        {
            ++count;
            if (count == n)
                result = dsum;               /* store digit sum of nth Niven */
        }
        ++num;
    }
    return result;
}
