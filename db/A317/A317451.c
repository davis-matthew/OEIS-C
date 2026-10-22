/*
 * A317451  a(n) = (n*A003500(n) - A231896(n))/2.
 * https://oeis.org/A317451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=2, a(2)=16, a(3)=92; for n>3,
 *            a(n)=8*a(n-1)-18*a(n-2)+8*a(n-3)-a(n-4)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial terms a(0) … a(3) */
    static const seq_t init[4] = {0, 2, 16, 92};

    if (n < 4)
        return init[(size_t)n];

    /* sliding window of the last four values, a(n‑4)…a(n‑1) */
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = (seq_t)8 * a3 - (seq_t)18 * a2 + (seq_t)8 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
