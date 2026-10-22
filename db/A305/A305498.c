/*
 * A305498  The smallest positive even integer that can be represented with n
 *          digits in base 3/2.
 * https://oeis.org/A305498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 2, a(n+1) = 2 * ceil(3 * a(n) / 4) (OEIS A305498)
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; values for n == 0 are
     * irrelevant for the tests.  For n == 0 we simply return 0
     * to keep the function well‑defined. */
    if (n == 0)
        return (seq_t)0;

    seq_t val = (seq_t)2;          /* a(1) */
    for (idx_t i = 1; i < n; ++i)
    {
        /* Compute ceil(3*val/4).  Using integer arithmetic, ceil(x/4)
         * is (x + 3)/4 for integer x.  Since val is always positive,
         * the following is safe and exact. */
        seq_t temp = val * (seq_t)3;
        seq_t ceil_val = (temp + (seq_t)3) / (seq_t)4;

        val = ceil_val * (seq_t)2;  /* a(i+1) */
    }
    return val;
}
