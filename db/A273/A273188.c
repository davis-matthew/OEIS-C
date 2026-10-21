/*
 * A273188  Numbers whose digit sum is divisible by 8.
 * https://oeis.org/A273188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 8189, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the nth non‑negative integer whose digit sum is
   divisible by 8 (the numbers listed in the OEIS b‑file). */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* In case the caller uses n = 0 (outside the OEIS offset), simply
       return 0 to avoid undefined behaviour. */
    if (n == 0) return (seq_t)0;

    idx_t    count = 0;   /* How many qualifying numbers have been found */
    seq_t    x     = 0;   /* Candidate number */

    /* Walk through the non‑negative integers, checking the digit sum
       modulo 8.  When we've found the nth such number, return it.   */
    while (1) {
        if ((digit_sum(x) % 8) == 0) {
            ++count;
            if (count == n) return x;
        }
        ++x;
    }
}
