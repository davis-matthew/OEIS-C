/*
 * A259597  Numerators of the other-side convergents to sqrt(7).
 * https://oeis.org/A259597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 16*a(n-4) - a(n-8) for n > 7; Initial values: a(0)=3,
 *            a(1)=5, a(2)=8, a(3)=13, a(4)=45, a(5)=82, a(6)=127, a(7)=209
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Base values for n = 0..7. */
    static const seq_t init[8] = {
        3, 5, 8, 13, 45, 82, 127, 209
    };

    /* For small indices just return the pre‑computed value. */
    if (n < 8)
        return init[(size_t)n];

    /* Use an 8‑element circular buffer to hold the last 8 terms. */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    /* Iterate up to the requested index. */
    for (idx_t i = 8; i <= n; ++i)
    {
        seq_t term4 = buf[(i - 4) & 7];
        seq_t term8 = buf[(i - 8) & 7];
        buf[i & 7] = (seq_t)16 * term4 - term8;
    }

    return buf[n & 7];
}
