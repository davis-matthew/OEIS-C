/*
 * A375692  Decimal concatenation of the 5 numbers n,n+1,n+2,n+3,n+4.
 * https://oeis.org/A375692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concat(n, n+1, n+2, n+3, n+4)
 * Range:     u8 none, u16 n <= 5, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned digit_len(idx_t x)
{
    unsigned len = 0;
    do {
        ++len;
        x /= 10;
    } while (x != 0);
    return len;
}

static inline seq_t pow10(unsigned e)
{
    seq_t r = 1;
    while (e--)
        r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    seq_t result = (seq_t)n;
    for (idx_t i = 1; i <= 4; ++i)
    {
        idx_t val    = n + i;
        unsigned d   = digit_len(val);
        result = result * pow10(d) + (seq_t)val;
    }
    return result;
}
