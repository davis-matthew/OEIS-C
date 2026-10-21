/*
 * A269147  Number of nX3 0..3 arrays with some element plus some
 *          horizontally, antidiagonally or vertically adjacent neighbor
 *          totalling three exactly once.
 * https://oeis.org/A269147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 26*a(n-1) –199*a(n-2) +408*a(n-3) –463*a(n-4) +322*a(n-5)
 *            –141*a(n-6) +36*a(n-7) –4*a(n-8) for n > 9 Initial values:
 *            a(1)=24, a(2)=768, a(3)=13904, a(4)=220968, a(5)=3277728
 *            a(6)=46576336, a(7)=642676704, a(8)=8680278136,
 *            a(9)=115349343264
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 9 initial terms */
    static const seq_t init[9] = {
        (seq_t)24,
        (seq_t)768,
        (seq_t)13904,
        (seq_t)220968,
        (seq_t)3277728,
        (seq_t)46576336,
        (seq_t)642676704,
        (seq_t)8680278136,
        (seq_t)115349343264
    };

    if (n <= 9)               /* offset   1  ... 9 */
        return init[n-1];

    /* prev[0] = a(k-1), prev[1] = a(k-2), ..., prev[7] = a(k-8)  */
    seq_t prev[8];
    for (int i = 0; i < 8; ++i)
        prev[i] = init[8 - i];          /* a(9), a(8), …, a(2) */

    seq_t cur = 0;
    for (idx_t k = 10; k <= n; ++k) {
        /* compute the recurrence using only additions and subtractions,
           after the positive part each term is guaranteed to be larger
           than the negative part (a(k) > 0). */
        seq_t pos = prev[0] * (seq_t)26
                  + prev[2] * (seq_t)408
                  + prev[4] * (seq_t)322
                  + prev[6] * (seq_t)36;
        seq_t neg = prev[1] * (seq_t)199
                  + prev[3] * (seq_t)463
                  + prev[5] * (seq_t)141
                  + prev[7] * (seq_t)4;
        cur = pos - neg;

        /* shift the window for next iteration */
        for (int i = 7; i > 0; --i)
            prev[i] = prev[i - 1];
        prev[0] = cur;
    }
    return cur;   /* cur now holds a(n) */
}
