/*
 * A273716  The number of peaks of width 1 (i.e., UHD configurations, where U
 *          = (0,1), H=(1,0), D=(0,-1)) in all bargraphs of semiperimeter n
 *          (n>=2).
 * https://oeis.org/A273716
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies (n-2)*(2*n-9)*a(n) + (2*n^2-29*n+75)*a(n-1)
 *            -6*(2*n-7)*(3*n-17)*a(n-2) +10*(2*n-9)*(n-5)*a(n-3)
 *            +(2*n-5)*(n-6)*a(n-4) +5*(2*n-7)*(n-7)*a(n-5) = 0 (n ≥ 7) with
 *            a(2)=1, a(3)=1, a(4)=3, a(5)=8, a(6)=23
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <inttypes.h> /* for int64_t */

seq_t a(idx_t n)
{
    /* Base values for n = 2..6 */
    static const seq_t base[7] = {0, 0, 1, 1, 3, 8, 23};

    if (n < 2) return 0;
    if (n <= 6) return base[n];

    /* previous values: prev1 = a(n-1), prev2 = a(n-2), ... */
    seq_t prev1 = base[6]; /* a(6) */
    seq_t prev2 = base[5]; /* a(5) */
    seq_t prev3 = base[4]; /* a(4) */
    seq_t prev4 = base[3]; /* a(3) */
    seq_t prev5 = base[2]; /* a(2) */

    seq_t cur_val = 0;

    for (idx_t cur = 7; cur <= n; ++cur) {
        /* coefficients – using int64_t for sign */
        int64_t c0 = (int64_t)(cur - 2) * (2 * (int64_t)cur - 9);
        int64_t c1 = 2 * (int64_t)cur * cur - 29 * (int64_t)cur + 75;
        int64_t c2 = -6 * (int64_t)(2 * cur - 7) * (3 * (int64_t)cur - 17);
        int64_t c3 = 10 * (int64_t)(2 * cur - 9) * (cur - 5);
        int64_t c4 = (2 * cur - 5) * (cur - 6);
        int64_t c5 = 5 * (int64_t)(2 * cur - 7) * (cur - 7);

        /* sum of positive and negative contributions */
        seq_t pos = 0;
        seq_t neg = 0;

        if (c1 > 0) pos += prev1 * (seq_t)c1;
        else if (c1 < 0) neg += prev1 * (seq_t)(-c1);

        if (c2 > 0) pos += prev2 * (seq_t)c2;
        else if (c2 < 0) neg += prev2 * (seq_t)(-c2);

        if (c3 > 0) pos += prev3 * (seq_t)c3;
        else if (c3 < 0) neg += prev3 * (seq_t)(-c3);

        if (c4 > 0) pos += prev4 * (seq_t)c4;
        else if (c4 < 0) neg += prev4 * (seq_t)(-c4);

        if (c5 > 0) pos += prev5 * (seq_t)c5;
        else if (c5 < 0) neg += prev5 * (seq_t)(-c5);

        /* a(cur) = (neg - pos) / c0  */
        cur_val = (neg - pos) / (seq_t)c0;

        /* shift previous values for next iteration */
        prev5 = prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur_val;
    }

    return cur_val;
}
