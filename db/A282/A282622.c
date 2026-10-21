/*
 * A282622  Number of digits of the representation of n in the alternating
 *          sexagesimal-decimal number system.
 * https://oeis.org/A282622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the number of digits of the representation of n in the
 *            alternating sexagesimal‑decimal number system. The thresholds
 *            for the digit lengths are obtained by start_1 = 1 start_{k+1} =
 *            start_k * 10 if k is odd = start_k * 6 + offset if k is even
 *            where offset = 0 for k = 2, and offset = 1 for k...
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t start = 1;          /* first index of the current digit length */
    unsigned int len  = 1;    /* current digit length */

    while (1)
    {
        /* Determine the multiplier and offset for the next threshold. */
        idx_t mult   = (len & 1) ? 10 : 6;          /* len odd -> 10, even -> 6 */
        idx_t offset = 0;
        if ((len & 1) == 0 && len > 2)             /* even length ≥4 */
            offset = 1;

        /* Compute next threshold safely, guarding against overflow. */
        if (start > (UINT64_MAX - offset) / mult)
            return (seq_t)len;          /* next would overflow and is > n */

        idx_t next = start * mult + offset;

        if (n < next)
            return (seq_t)len;          /* n lies in the current length */

        /* Advance to the next length. */
        start = next;
        ++len;
    }
}
