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
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_nonprime_digit(seq_t x)
{
    /* Zero has a non‑prime digit (0). */
    if (x == 0)
        return true;

    while (x > 0)
    {
        int d = (int)(x % 10);
        if (d == 0 || d == 1 || d == 4 || d == 6 || d == 8 || d == 9)
            return true;
        x /= 10;
    }
    return false;
}

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
