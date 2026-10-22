/*
 * A300961  Number of n X 3 0..1 arrays with every element equal to 0, 2, 3, 4
 *          or 6 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A300961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +5*a(n-2) +2*a(n-3) -30*a(n-4) -29*a(n-5)
 *            -3*a(n-6) +26*a(n-7) +89*a(n-8) +79*a(n-9) -20*a(n-10)
 *            -52*a(n-11) -28*a(n-12) +96*a(n-13) +12*a(n-14) +4*a(n-15)
 *            -39*a(n-16) -17*a(n-17) -4*a(n-18) -3*a(n-19) +7*a(n-20)
 *            -3*a(n-21) +2*a(n-22) for n > 24. Initial values a(1) = 1, a(2)
 *            = 5,...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

static const seq_t init_vals[24] = {
    (seq_t)1ULL, (seq_t)5ULL, (seq_t)16ULL, (seq_t)44ULL, (seq_t)159ULL, (seq_t)580ULL,
    (seq_t)2145ULL, (seq_t)8034ULL, (seq_t)30017ULL, (seq_t)113054ULL, (seq_t)426489ULL,
    (seq_t)1609240ULL, (seq_t)6078735ULL, (seq_t)22967236ULL, (seq_t)86790502ULL,
    (seq_t)328025891ULL, (seq_t)1239827639ULL, (seq_t)4686314366ULL, (seq_t)17713839936ULL,
    (seq_t)66957270615ULL, (seq_t)253096517486ULL, (seq_t)956701365708ULL,
    (seq_t)3616324822124ULL, (seq_t)13669703752446ULL
};

static const int coeff[22] = {
    3, 5, 2, -30, -29, -3, 26, 89, 79, -20,
    -52, -28, 96, 12, 4, -39, -17, -4, -3, 7, -3, 2
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined, safe guard */

    if (n <= 24) return init_vals[n - 1];

    /* If n > 24, compute iteratively using the recurrence.  Use a circular
       buffer that holds the last 22 terms: buf[0] = a(i-1), buf[1] = a(i-2), ... */
    seq_t buf[22];
    /* Initialize buffer for i = 25 : buf[0] = a(24), buf[1] = a(23), ... buf[21] = a(3). */
    for (int k = 0; k < 22; ++k) {
        buf[k] = init_vals[24 - 1 - k];           /* init_vals[23] is a(24) */
    }

    /* Compute from i = 25 up to n. */
    for (idx_t i = 25; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 22; ++k) {
            /* absolute value of coefficient */
            unsigned int m = (unsigned int)(coeff[k] > 0 ? coeff[k] : -coeff[k]);
            seq_t term = mul_small(buf[k], m);
            if (coeff[k] > 0)
                pos += term;
            else
                neg += term;
        }
        /* pos >= neg for all n, because the recurrence yields positive values. */
        seq_t res = pos - neg;

        /* Shift buffer: drop the oldest value and insert the new result. */
        for (int k = 21; k > 0; --k)
            buf[k] = buf[k - 1];
        buf[0] = res;
    }

    return buf[0];
}
