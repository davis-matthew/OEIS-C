/*
 * A320636  Negative numbers in base -3.
 * https://oeis.org/A320636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal digits of the base (-3) representation of -n. The
 *            sequence gives the representation of the negative integer -n in
 *            base -3. Example: a(7) = 1202, since 1202 (base -3) = -7
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* Not defined, but harmless */
    long long x = -(long long)n;     /* Negative value to convert */
    seq_t res = 0;
    long long power = 1;             /* 10^digit position */

    while (x != 0)
    {
        long long rem = x % -3;      /* Remainder may be negative */
        x /= -3;                    /* Quotient */
        if (rem < 0)                /* Adjust to bring remainder into [0,2] */
        {
            rem += 3;
            x += 1;
        }
        res += (seq_t)rem * power;   /* Append digit in decimal */
        power *= 10;
    }
    return res;
}
