/*
 * A288916  Number of 5-cycles in the n X n queen graph.
 * https://oeis.org/A288916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 7*a(n-3) - 4*a(n-4) + 9*a(n-5) +
 *            6*a(n-6) - 6*a(n-7) - 9*a(n-8) + 4*a(n-9) + 7*a(n-10) -
 *            3*a(n-11) - 2*a(n-12) + a(n-13) for n ≥ 14, with initial values:
 *            a(1)=0, a(2)=0, a(3)=376, a(4)=4644, a(5)=23920, a(6)=81876,
 *            a(7)=219384, a(8)=499544, a(9)=1014160, a(10)=1885960...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* recurrence coefficients for a(n) in terms of a(n-1)..a(n-13) */
static const int c_coef[13] = { 2, 3, -7, -4, 9, 6, -6, -9, 4, 7, -3, -2, 1 };

/* initial values a(1)..a(13) */
static const seq_t init_vals[14] = {
    0,          /* unused 0‑th element */
    0,          /* a(1) */
    0,          /* a(2) */
    376,        /* a(3) */
    4644,       /* a(4) */
    23920,      /* a(5) */
    81876,      /* a(6) */
    219384,     /* a(7) */
    499544,     /* a(8) */
    1014160,    /* a(9) */
    1885960,    /* a(10) */
    3280968,    /* a(11) */
    5402716,    /* a(12) */
    8516848     /* a(13) */
};

seq_t a(idx_t n)
{
    if (n < 1u)                       /* undefined for n < 1 */
        return 0;

    if (n <= 13u)                     /* initial values */
        return init_vals[ (size_t)n ];

    /* Keep the last 13 computed terms: prev[0] = a(i-13), prev[12] = a(i-1) */
    seq_t prev[13];
    for (int i = 0; i < 13; ++i)
        prev[i] = init_vals[i + 1];   /* prev[0] = a(1), ..., prev[12] = a(13) */

    size_t limit = (size_t)n;
    for (size_t i = 14; i <= limit; ++i) {
        seq_t sum_pos = 0, sum_neg = 0;
        for (int k = 0; k < 13; ++k) {
            seq_t term = prev[12 - k];            /* a(i - (k+1)) */
            int coeff = c_coef[k];
            if (coeff > 0)
                sum_pos += term * (seq_t)coeff;
            else if (coeff < 0)
                sum_neg += term * (seq_t)(-coeff);
        }
        seq_t nxt = sum_pos - sum_neg;            /* guaranteed non‑negative */

        /* shift prev left by one, drop a(i-13), and store new term */
        for (int j = 0; j < 12; ++j)
            prev[j] = prev[j + 1];
        prev[12] = nxt;
    }

    return prev[12];   /* a(n) */
}
