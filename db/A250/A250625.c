/*
 * A250625  Number of (n+1) X (1+1) 0..2 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + a(n-3) + 16*a(n-4) - 10*a(n-5) -
 *            7*a(n-6) + 6*a(n-7) + a(n-8) - a(n-9) Initial values: a(1)=36,
 *            a(2)=125, a(3)=380, a(4)=1072, a(5)=2856, a(6)=7307, a(7)=18131,
 *            a(8)=43966, a(9)=104755
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 21, u64 n <= 49, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t init[9] = {
        (seq_t)36,   /* a(1) */
        (seq_t)125,  /* a(2) */
        (seq_t)380,  /* a(3) */
        (seq_t)1072, /* a(4) */
        (seq_t)2856, /* a(5) */
        (seq_t)7307, /* a(6) */
        (seq_t)18131,/* a(7) */
        (seq_t)43966,/* a(8) */
        (seq_t)104755/* a(9) */
    };

    if (n <= 9) {
        return init[n - 1];
    }

    /* Sliding window for the last 9 terms:
       v1 = a(n-9), v2 = a(n-8), …, v9 = a(n-1)   */
    seq_t v1 = init[0], v2 = init[1], v3 = init[2],
          v4 = init[3], v5 = init[4], v6 = init[5],
          v7 = init[6], v8 = init[7], v9 = init[8];

    for (idx_t i = 10; i <= n; ++i) {
        /* Compute a(i) from the recurrence */
        seq_t pos = 6 * v9 + v7 + 16 * v6 + 6 * v3 + v2;
        seq_t neg = 11 * v8 + 10 * v5 + 7 * v4 + v1;
        seq_t val = pos - neg;

        /* Shift the window for the next step */
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = v5;
        v5 = v6;
        v6 = v7;
        v7 = v8;
        v8 = v9;
        v9 = val;
    }

    return v9;
}
