/*
 * A314055  Coordination sequence Gal.4.143.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-5) + a(n-7) - a(n-12) for n > 12 a(0) = 1, a(1) = 5,
 *            a(2) = 11, a(3) = 15, a(4) = 21, a(5) = 26, a(6) = 33, a(7) =
 *            37, a(8) = 41, a(9) = 47, a(10) = 52, a(11) = 59, a(12) = 63
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


seq_t a(idx_t n)
{
    static const seq_t init[13] = {
        (seq_t)1  , (seq_t)5  , (seq_t)11 , (seq_t)15 , (seq_t)21 ,
        (seq_t)26 , (seq_t)33 , (seq_t)37 , (seq_t)41 , (seq_t)47 ,
        (seq_t)52 , (seq_t)59 , (seq_t)63
    };

    if (n <= 12)
        return init[(size_t)n];

    seq_t buf[13];
    for (int i = 0; i < 13; ++i)
        buf[i] = init[i];

    for (size_t i = 13; i <= (size_t)n; ++i) {
        int a_i = (int)((i - 5) % 13);
        int b_i = (int)((i - 7) % 13);
        int c_i = (int)((i - 12) % 13);

        seq_t next = buf[a_i] + buf[b_i] - buf[c_i];
        buf[(int)(i % 13)] = next;
    }

    return buf[(int)(n % 13)];
}
