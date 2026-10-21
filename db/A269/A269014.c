/*
 * A269014  Number of 4 X n binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A269014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) + 28*a(n-2) - 78*a(n-3) - 264*a(n-4)
 *                    + 296*a(n-5) + 527*a(n-6) - 252*a(n-7) - 324*a(n-8)
 * Base values (1‑based): a(1)=0, a(2)=48, a(3)=224, a(4)=2136,
 *                        a(5)=10976, a(6)=73568, a(7)=390064, a(8)=2291728 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values, 0th entry unused */
    static const seq_t base[9] = {
        (seq_t)0,
        (seq_t)0,    /* a(1) */
        (seq_t)48,   /* a(2) */
        (seq_t)224,  /* a(3) */
        (seq_t)2136, /* a(4) */
        (seq_t)10976,/* a(5) */
        (seq_t)73568,/* a(6) */
        (seq_t)390064,/* a(7) */
        (seq_t)2291728/* a(8) */
    };

    if (n <= 1)
        return base[1];
    if (n <= 8)
        return base[n];

    /* Rolling window for the last 8 terms: w[0] = a(i-1), w[1] = a(i-2), ..., w[7] = a(i-8) */
    seq_t w[8];
    w[0] = base[8];
    w[1] = base[7];
    w[2] = base[6];
    w[3] = base[5];
    w[4] = base[4];
    w[5] = base[3];
    w[6] = base[2];
    w[7] = base[1];

    for (idx_t i = 9; i <= n; ++i) {
        seq_t val = 4 * w[0] + 28 * w[1] - 78 * w[2] - 264 * w[3]
                    + 296 * w[4] + 527 * w[5] - 252 * w[6] - 324 * w[7];

        /* Shift window right: w[7] = w[6], ..., w[1] = w[0] */
        for (int k = 7; k > 0; --k)
            w[k] = w[k - 1];
        w[0] = val;  /* w[0] now holds a(i) */
    }

    return w[0];   /* a(n) */
}
