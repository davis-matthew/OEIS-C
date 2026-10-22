/*
 * A302654  Number of minimum total dominating sets in the n-path graph.
 * https://oeis.org/A302654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-4) - 3*a(n-8) + a(n-12) Initial values (for 1 ≤ n ≤
 *            12): a(1)=0, a(2)=1, a(3)=2, a(4)=1, a(5)=1, a(6)=4, a(7)=3,
 *            a(8)=1, a(9)=2, a(10)=9, a(11)=4, a(12)=1
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* base terms */
    static const seq_t base[13] = {
        0,  /* base[0] unused */
        0, 1, 2, 1, 1, 4, 3, 1, 2, 9, 4, 1
    };

    if (n <= 12)
        return base[(size_t)n];

    /* buffer for last 13 computed values; indices 0..12
       base[1..12] stored beforehand. */
    seq_t buf[13];
    for (size_t i = 1; i <= 12; ++i)
        buf[i] = base[i];
    buf[0] = 0;  /* placeholder, will be overwritten at i = 13 */

    for (idx_t i = 13; i <= n; ++i) {
        size_t idx  = (size_t)(i % 13);
        size_t idx4 = (size_t)((i - 4) % 13);
        size_t idx8 = (size_t)((i - 8) % 13);
        size_t idx12= (size_t)((i -12) % 13);

        seq_t term1 = buf[idx4] * 3;
        seq_t term2 = buf[idx8] * 3;
        seq_t diff  = term1 - term2;           /* non‑negative by definition */
        buf[idx]    = diff + buf[idx12];
    }

    return buf[(size_t)(n % 13)];
}
