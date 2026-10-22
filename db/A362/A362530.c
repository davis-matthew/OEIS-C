/*
 * A362530  Decimal expansion of the conventional value of Josephson constant
 *          (K_{J-90}) in Hz/V.
 * https://oeis.org/A362530
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  cons, nonn, fini, full
 * Formula:   a(n) = the (n-15)th decimal digit of 483597900000000, defined
 *            for n ≥ 15; 0 for n < 15 or n > 29
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 15) return (seq_t)0;          /* below offset is undefined */

    idx_t pos = n - 15;                    /* position in the 15-digit string */

    if (pos < 15)                          /* valid digit? */
    {
        static const char s[] = "483597900000000";
        return (seq_t)(s[(size_t)pos] - '0');
    }

    return (seq_t)0;                       /* beyond the known digits */
}
