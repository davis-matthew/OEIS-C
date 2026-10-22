/*
 * A310683  Coordination sequence Gal.4.15.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) + a(n-6) -
 *            a(n-7) + a(n-8) - a(n-9) + a(n-10) - a(n-11) for n > 11. Initial
 *            values: a(0)=1, a(1)=4, a(2)=7, a(3)=10, a(4)=13, a(5)=16,
 *            a(6)=22, a(7)=26, a(8)=27, a(9)=30, a(10)=33, a(11)=36
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* the twelve known terms */
    static const seq_t init[12] = {
        (seq_t)1, (seq_t)4, (seq_t)7, (seq_t)10, (seq_t)13,
        (seq_t)16, (seq_t)22, (seq_t)26, (seq_t)27, (seq_t)30,
        (seq_t)33, (seq_t)36
    };

    if (n < 12)
        return init[(idx_t)n];

    /* ring buffer of the last 12 terms */
    seq_t buf[12];
    for (int i = 0; i < 12; ++i)
        buf[i] = init[i];

    /* compute forward up to n */
    for (idx_t i = 12; i <= n; ++i)
    {
        /* indices modulo 12 */
        idx_t a1 = (i - 1) % 12;
        idx_t a2 = (i - 2) % 12;
        idx_t a3 = (i - 3) % 12;
        idx_t a4 = (i - 4) % 12;
        idx_t a5 = (i - 5) % 12;
        idx_t a6 = (i - 6) % 12;
        idx_t a7 = (i - 7) % 12;
        idx_t a8 = (i - 8) % 12;
        idx_t a9 = (i - 9) % 12;
        idx_t a10 = (i -10) % 12;
        idx_t a11 = (i -11) % 12;

        seq_t val = buf[a1];
        val -= buf[a2];
        val += buf[a3];
        val -= buf[a4];
        val += buf[a5];
        val += buf[a6];
        val -= buf[a7];
        val += buf[a8];
        val -= buf[a9];
        val += buf[a10];
        val -= buf[a11];

        buf[i % 12] = val;
    }

    return buf[n % 12];
}
