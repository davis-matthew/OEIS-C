/*
 * A261337  Digit-sums in an incremental base that adjusts itself as the
 *          digits of n are generated from right to left.
 * https://oeis.org/A261337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit sum of n in an incremental base that starts at 2
 *            and increases by the digit just generated (right‑to‑left). For n
 *            = 0, a(0) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for exact integer types if needed */

seq_t a(idx_t n)
{
    if (n == 0)               /* a(0) = 0 */
        return 0;

    seq_t sum   = 0;          /* accumulation of digits */
    idx_t base  = 2;          /* starting base */

    /* Process digits from least significant to most significant */
    while (n != 0) {
        seq_t digit = (seq_t)(n % base);   /* current digit */
        sum += digit;

        n /= base;                     /* integer division */
        base += (idx_t)digit;           /* increment base by current digit */
    }

    return sum;
}
