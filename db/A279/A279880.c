/*
 * A279880  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 213", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A279880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A279880 a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=28, a(5)=7,
 *            a(6)=120, a(7)=15, a(8)=496, a(9)=31, a(10)=2016, a(11)=63,
 *            a(12)=8128. For n≥13: a(n) = 7*a(n-2) – 14*a(n-4) + 8*a(n-6)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[13] = {
        (seq_t)1,(seq_t)1,(seq_t)2,(seq_t)3,(seq_t)28,(seq_t)7,(seq_t)120,
        (seq_t)15,(seq_t)496,(seq_t)31,(seq_t)2016,(seq_t)63,(seq_t)8128
    };
    if (n < 13) return init[n];

    /* circular buffer for the last 7 values (indices modulo 7) */
    seq_t buf[7];
    for (int i = 0; i < 7; ++i) buf[i] = init[i];

    /* fill buffer for indices 7..12 with the known constants */
    for (idx_t i = 7; i <= 12; ++i) buf[i % 7] = init[i];

    for (idx_t i = 13; i <= n; ++i) {
        /* a(n) = 7*a(n-2) – 14*a(n-4) + 8*a(n-6) */
        seq_t val = (seq_t)7 * buf[(i - 2) % 7]
                - (seq_t)14 * buf[(i - 4) % 7]
                + (seq_t)8 * buf[(i - 6) % 7];
        buf[i % 7] = val;
    }

    return buf[n % 7];
}
