/*
 * A301961  Number of nX5 0..1 arrays with every element equal to 0, 1 or 4
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - a(n-3) - 2*a(n-4) - 3*a(n-5) +
 *            2*a(n-6) for n>8 Initial values: a(1)=8, a(2)=14, a(3)=41,
 *            a(4)=111, a(5)=311, a(6)=874, a(7)=2454, a(8)=6906
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 1‑based array of the first 8 terms (index 0 unused) */
    static const seq_t init[9] = {
        (seq_t)0,    /* dummy for index 0 */
        (seq_t)8,    /* a(1) */
        (seq_t)14,   /* a(2) */
        (seq_t)41,   /* a(3) */
        (seq_t)111,  /* a(4) */
        (seq_t)311,  /* a(5) */
        (seq_t)874,  /* a(6) */
        (seq_t)2454, /* a(7) */
        (seq_t)6906  /* a(8) */
    };

    if (n <= 8) {
        return init[n];
    }

    /* Sliding window of the last six values:
       v[0] = a[i-6], v[1] = a[i-5], ..., v[5] = a[i-1]  (for i currently invalid)
    */
    seq_t v[6];
    v[0] = init[3];
    v[1] = init[4];
    v[2] = init[5];
    v[3] = init[6];
    v[4] = init[7];
    v[5] = init[8];

    seq_t val = 0;
    idx_t i;
    for (i = 9; i <= n; ++i) {
        /* recurrence */
        val = 2 * v[5] + 3 * v[4] - v[3] - 2 * v[2] - 3 * v[1] + 2 * v[0];

        /* shift window for next iteration */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = val;
    }
    return val;
}
