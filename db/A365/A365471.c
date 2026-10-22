/*
 * A365471  Numbers whose digits are not all primes.
 * https://oeis.org/A365471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th nonnegative integer that contains at least one
 *            decimal digit that is NOT prime (i.e. contains 0,1,4,6,8,9).
 *            Equivalent to numbers that are NOT composed solely of the prime
 *            digits 2,3,5,7. The sequence starts 0,1,4,6,8,9,10,11,
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   has_nonprime_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_nonprime_digit.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t x = 0;

    while (1)
    {
        if (has_nonprime_digit(x))
        {
            ++count;
            if (count == n)
                return x;
        }
        ++x;
    }
}
