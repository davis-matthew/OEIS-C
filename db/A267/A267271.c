/*
 * A267271  Decimal representation of the n-th iteration of the "Rule 115"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*a(n-2) - 152*a(n-4) + 128*a(n-6) for n > 6 Initial
 *            values: a(0)=1, a(1)=5, a(2)=12, a(3)=111, a(4)=96, a(5)=1919,
 *            a(6)=768
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Keep the first seven terms in a circular buffer of length 7. */
    static const seq_t init[7] = { (seq_t)1, (seq_t)5, (seq_t)12,
                                   (seq_t)111, (seq_t)96, (seq_t)1919,
                                   (seq_t)768 };

    if (n < 7)
        return init[n];

    seq_t buf[7];
    for (size_t i = 0; i < 7; ++i)
        buf[i] = init[i];

    /* Compute terms from 7 up to n */
    for (idx_t i = 7; i <= n; ++i) {
        idx_t i_mod   = (size_t)i % 7;
        idx_t i2_mod  = (size_t)(i - 2) % 7;
        idx_t i4_mod  = (size_t)(i - 4) % 7;
        idx_t i6_mod  = (size_t)(i - 6) % 7;

        seq_t term = 25 * buf[i2_mod] + 128 * buf[i6_mod];
        term -= 152 * buf[i4_mod];

        buf[i_mod] = term;
    }

    return buf[(size_t)n % 7];
}
