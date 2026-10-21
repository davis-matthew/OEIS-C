/*
 * A268907  Number of 4Xn 0..2 arrays with some element plus some horizontally
 *          or antidiagonally adjacent neighbor totalling two exactly once.
 * https://oeis.org/A268907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) -111*a(n-2) +282*a(n-3) -333*a(n-4) +180*a(n-5)
 *            -36*a(n-6), for n > 12. Initial values: a(1)=0, a(2)=1344,
 *            a(3)=12960, a(4)=118584, a(5)=1004184, a(6)=8250912,
 *            a(7)=66210264, a(8)=522241560, a(9)=4063962024,
 *            a(10)=31282792704, a(11)=238663638432, a(12)=1807307152056
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* pre‑computed values for n = 1 … 12 (index 0 unused) */
    static const seq_t init[13] = {
        0ULL,          /* init[0] unused */
        0ULL,          /* a(1) */
        1344ULL,       /* a(2) */
        12960ULL,      /* a(3) */
        118584ULL,     /* a(4) */
        1004184ULL,    /* a(5) */
        8250912ULL,    /* a(6) */
        66210264ULL,   /* a(7) */
        522241560ULL,  /* a(8) */
        4063962024ULL, /* a(9) */
        31282792704ULL,/* a(10) */
        238663638432ULL,/* a(11) */
        1807307152056ULL/* a(12) */
    };

    if (n <= 12)
        return init[n];

    /* ring buffer holding the last six terms: buf[0] = a(k-1),
       buf[1] = a(k-2), … buf[5] = a(k-6) */
    seq_t buf[6];
    buf[0] = init[12];
    buf[1] = init[11];
    buf[2] = init[10];
    buf[3] = init[9];
    buf[4] = init[8];
    buf[5] = init[7];

    for (idx_t k = 13; k <= n; ++k) {
        seq_t pos = mul_small(buf[0], 18);
        pos += mul_small(buf[2], 282);
        pos += mul_small(buf[4], 180);

        seq_t neg = mul_small(buf[1], 111);
        neg += mul_small(buf[3], 333);
        neg += mul_small(buf[5], 36);

        seq_t next = pos - neg;   /* pos >= neg by the recurrence */

        /* shift buffer for next iteration */
        buf[5] = buf[4];
        buf[4] = buf[3];
        buf[3] = buf[2];
        buf[2] = buf[1];
        buf[1] = buf[0];
        buf[0] = next;
    }

    return buf[0];
}
