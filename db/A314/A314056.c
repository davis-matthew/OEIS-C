/*
 * A314056  Coordination sequence Gal.4.135.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=5, a(2)=11, a(3)=15, a(4)=21, a(5)=27, a(6)=31,
 *            a(7)=37, a(8)=42 a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) -
 *            2*a(n-4) + 2*a(n-5) - 2*a(n-6) + 2*a(n-7) - a(n-8) for n > 8
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial 9 terms */
    static const seq_t init[9] = {
        (seq_t)1, (seq_t)5, (seq_t)11, (seq_t)15, (seq_t)21,
        (seq_t)27, (seq_t)31, (seq_t)37, (seq_t)42
    };

    if (n < 9) {
        return init[n];
    }

    /* Circular buffer of the last 9 computed values */
    seq_t buf[9];
    for (int i = 0; i < 9; ++i) {
        buf[i] = init[i];
    }

    for (idx_t i = 9; i <= n; ++i) {
        /* indices modulo 9 */
        size_t i1 = (i + 8) % 9; /* (i-1) % 9 */
        size_t i3 = (i + 6) % 9; /* (i-3) % 9 */
        size_t i5 = (i + 4) % 9; /* (i-5) % 9 */
        size_t i7 = (i + 2) % 9; /* (i-7) % 9 */
        size_t i2 = (i + 7) % 9; /* (i-2) % 9 */
        size_t i4 = (i + 5) % 9; /* (i-4) % 9 */
        size_t i6 = (i + 3) % 9; /* (i-6) % 9 */
        size_t i8 = (i + 1) % 9; /* (i-8) % 9 */

        seq_t pos = (seq_t)2 * buf[i1] + (seq_t)2 * buf[i3]
                  + (seq_t)2 * buf[i5] + (seq_t)2 * buf[i7];
        seq_t neg = (seq_t)2 * buf[i2] + (seq_t)2 * buf[i4]
                  + (seq_t)2 * buf[i6] + buf[i8];
        buf[i % 9] = pos - neg;
    }

    return buf[n % 9];
}
