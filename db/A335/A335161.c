/*
 * A335161  a(n) = nim-product of 2^4 and 2^n.
 * https://oeis.org/A335161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nim-product of 2^4 and 2^n Recurrence: a(n) = 256*a(n-8)
 *            for n >= 8 Initial values: a(0)=16, a(1)=32, a(2)=64, a(3)=128,
 *            a(4)=24, a(5)=44, a(6)=75, a(7)=141
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Base values for n=0..7 */
    static const seq_t init[8] = {
        (seq_t)16,
        (seq_t)32,
        (seq_t)64,
        (seq_t)128,
        (seq_t)24,
        (seq_t)44,
        (seq_t)75,
        (seq_t)141
    };

    seq_t r = init[n & 7];            /* n % 8 */
    idx_t q = n >> 3;                /* n / 8 */

    if (q == 0)
        return r;                    /* n < 8 */

    /* 256^q = 2^(8q) */
    seq_t mult = big_pow((seq_t)256, (seq_t)q);

    return r * mult;                 /* multiplication is exact for any seq_t */
}
