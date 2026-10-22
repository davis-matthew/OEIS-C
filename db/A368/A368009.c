/*
 * A368009  Decimal expansion of 53/31, being the highest possible points
 *          won/lost ratio in a completed 3-set tennis match which the player
 *          loses.
 * https://oeis.org/A368009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   decimal digits of 53/31 (period 15): a(1)=1, for n≥2
 *            a(n)=pat[(n-2) mod 15] where pat =
 *            {7,0,9,6,7,7,4,1,9,3,5,4,8,3,8}
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* repeating block of digits */
    static const seq_t pat[15] = {
        (seq_t)7,(seq_t)0,(seq_t)9,(seq_t)6,(seq_t)7,
        (seq_t)7,(seq_t)4,(seq_t)1,(seq_t)9,(seq_t)3,
        (seq_t)5,(seq_t)4,(seq_t)8,(seq_t)3,(seq_t)8
    };

    idx_t pos = (n - 2) % 15; /* 0‑based index within the period */
    return pat[(size_t)pos];
}
