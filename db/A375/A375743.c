/*
 * A375743  Decimal expansion of 30547/21600.
 * https://oeis.org/A375743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n‑th decimal digit of 30547/21600 (including the
 *            integer part, so a(1)=1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants in uint64_t to avoid truncation when cast to seq_t */
    const uint64_t NUM = 30547;
    const uint64_t DEN = 21600;

    /* undefined for n==0 – return 0 */
    if (n == 0) return 0;

    /* the first term is the integer part of the fraction */
    if (n == 1) return (seq_t)(NUM / DEN);

    /* remainder after extracting the integer part */
    seq_t rem = (seq_t)(NUM % DEN);

    /* advance n‑2 decimal digits */
    for (idx_t i = 2; i < n; ++i)
    {
        rem = (rem * 10) % (seq_t)DEN;
    }

    /* compute the nth decimal digit */
    rem = rem * 10;
    seq_t digit = rem / (seq_t)DEN;
    return digit;
}
