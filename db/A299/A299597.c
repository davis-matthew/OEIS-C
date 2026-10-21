/*
 * A299597  Number of nX3 0..1 arrays with every element equal to 2, 3, 4, 5,
 *          7 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A299597:
 * a(n) = 5*a(n-1)+6*a(n-2)-31*a(n-3)-35*a(n-4)+102*a(n-5)+81*a(n-6)
 *       -227*a(n-7)-101*a(n-8)+221*a(n-9)+74*a(n-10)-34*a(n-11)-2*a(n-12)-8*a(n-13)
 * for n > 15, with a(1)=0, a(2)=3, a(3)=9, a(4)=27, a(5)=147, a(6)=643,
 * a(7)=3051, a(8)=14539, a(9)=69087, a(10)=330659, a(11)=1581181,
 * a(12)=7568599, a(13)=36237471, a(14)=173510451, a(15)=830875967.
 */

#include <stddef.h>  /* size_t */

seq_t a(idx_t n)
{
    /* Basic checks */
    if (n == 0) return (seq_t)0;  /* undefined for n=0, return 0 */

    /* Initial 15 terms of the sequence (a(1) .. a(15)) */
    static const seq_t init[15] = {
        (seq_t)0,                /* a(1) */
        (seq_t)3,                /* a(2) */
        (seq_t)9,                /* a(3) */
        (seq_t)27,               /* a(4) */
        (seq_t)147,              /* a(5) */
        (seq_t)643,              /* a(6) */
        (seq_t)3051,             /* a(7) */
        (seq_t)14539,            /* a(8) */
        (seq_t)69087,            /* a(9) */
        (seq_t)330659,           /* a(10) */
        (seq_t)1581181,          /* a(11) */
        (seq_t)7568599,          /* a(12) */
        (seq_t)36237471,         /* a(13) */
        (seq_t)173510451,        /* a(14) */
        (seq_t)830875967         /* a(15) */
    };

    if (n <= 15) {
        /* Return the corresponding pre‑computed value */
        return init[(size_t)(n - 1)];
    }

    /* Buffer to hold the last 13 computed terms: last[0] = a(i-1),
     * last[1] = a(i-2), ..., last[12] = a(i-13).  Initially they
     * contain a(15) .. a(3) corresponding to i = 16. */
    seq_t last[13];
    for (size_t k = 0; k < 13; ++k) {
        last[k] = init[14 - k];  /* 14-0 = 14 -> a(15), ... , 14-12 = 2 -> a(3) */
    }

    /* Iteratively compute a(n) using the recurrence */
    for (idx_t i = 16; i <= n; ++i) {
        seq_t pos = 5 * last[0] + 6 * last[1] + 102 * last[4] +
                    81 * last[5] + 221 * last[8] + 74 * last[9];
        seq_t neg = 31 * last[2] + 35 * last[3] + 227 * last[6] +
                    101 * last[7] + 34 * last[10] + 2 * last[11] + 8 * last[12];
        seq_t new_val = pos - neg;

        /* Shift the buffer to make room for the new term */
        for (size_t k = 12; k > 0; --k) {
            last[k] = last[k - 1];
        }
        last[0] = new_val;
    }

    return last[0];  /* a(n) */
}
