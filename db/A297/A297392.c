/*
 * A297392  Number of nX5 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1 neighboring 1.
 * https://oeis.org/A297392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +8*a(n-2) +27*a(n-3) +77*a(n-4) +164*a(n-5)
 *            -4*a(n-6) -124*a(n-7) -71*a(n-8) -231*a(n-9) -293*a(n-10)
 *            -114*a(n-11) -16*a(n-12) +2*a(n-13) +40*a(n-14) +98*a(n-15)
 *            +32*a(n-16) +16*a(n-17) +5*a(n-18) +a(n-19) -a(n-20). Initial
 *            values: a(1)=6, a(2)=33, a(3)=127, a(4)=700, a(5)=3175, a(6...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const int coeff[20] = {
    1, 8, 27, 77, 164, -4, -124, -71, -231, -293,
   -114, -16, 2, 40, 98, 32, 16, 5, 1, -1
};

static const seq_t init[20] = {
    6ULL, 33ULL, 127ULL, 700ULL, 3175ULL, 15541ULL,
    74736ULL, 358341ULL, 1729617ULL, 8308924ULL,
    40006463ULL, 192481249ULL, 926168176ULL,
    4456879585ULL, 21445119255ULL, 103193473908ULL,
    496552428929ULL, 2389355832929ULL, 11497320733428ULL,
    55323763675221ULL
};

seq_t a(idx_t n)
{
    if (n <= 20) return init[n - 1];

    /* buffer holds a(n-k) at index k (k=0..19) */
    seq_t buf[20];
    for (int i = 0; i < 20; ++i)
        buf[i] = init[19 - i];   /* buf[0]=a(20), buf[1]=a(19), …, buf[19]=a(1) */

    for (idx_t i = 21; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 0; k < 20; ++k) {
            seq_t val = buf[k];
            if (coeff[k] > 0)
                pos += (seq_t)coeff[k] * val;
            else
                neg += (seq_t)(-coeff[k]) * val;
        }
        seq_t next = pos - neg;   /* pos >= neg for valid recurrence */

        /* shift buffer left: discard oldest, insert newest at buf[0] */
        for (int j = 19; j > 0; --j)
            buf[j] = buf[j-1];
        buf[0] = next;
    }

    return buf[0];   /* buf[0] holds a(n) after loop */
}
