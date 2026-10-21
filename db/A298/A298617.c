/*
 * A298617  Number of nX3 0..1 arrays with every element equal to 1, 3, 4, 5
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by the given 19‑term linear recurrence for n > 20:
 *            a(n) = a(n-1) + 4*a(n-2) + 8*a(n-3) -10*a(n-4) -26*a(n-5)
 *            -6*a(n-6) +23*a(n-7) +19*a(n-8) -33*a(n-9) +42*a(n-10)
 *            +54*a(n-11) -56*a(n-12) -61*a(n-13) +24*a(n-14) +29*a(n-15)
 *            -4*a(n-16) + a(n-17) - a(n-18) - a(n-19) Initial values (OE...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* for abort() if desired */

static const seq_t init[21] = {
    0ULL,            /* dummy for index 0 */
    0ULL,            /* a(1) */
    4ULL,            /* a(2) */
    0ULL,            /* a(3) */
    7ULL,            /* a(4) */
    7ULL,            /* a(5) */
    25ULL,           /* a(6) */
    88ULL,           /* a(7) */
    187ULL,          /* a(8) */
    491ULL,          /* a(9) */
    1401ULL,         /* a(10) */
    3453ULL,         /* a(11) */
    9137ULL,         /* a(12) */
    24444ULL,        /* a(13) */
    63122ULL,        /* a(14) */
    165783ULL,       /* a(15) */
    437236ULL,       /* a(16) */
    1142360ULL,      /* a(17) */
    2996745ULL,      /* a(18) */
    7872571ULL,      /* a(19) */
    20629138ULL      /* a(20) */
};

seq_t a(idx_t n)
{
    if (n <= 20)          /* Return pre‑computed small terms */
        return init[(size_t)n];

    /* Store the 19 previous values needed for the recurrence.
       last[0] = a(i-1), last[1] = a(i-2), ..., last[18] = a(i-19)   */
    seq_t last[19];
    /* Initialize with a(20) .. a(2) (descending) */
    last[0] = init[20];
    last[1] = init[19];
    last[2] = init[18];
    last[3] = init[17];
    last[4] = init[16];
    last[5] = init[15];
    last[6] = init[14];
    last[7] = init[13];
    last[8] = init[12];
    last[9] = init[11];
    last[10] = init[10];
    last[11] = init[9];
    last[12] = init[8];
    last[13] = init[7];
    last[14] = init[6];
    last[15] = init[5];
    last[16] = init[4];
    last[17] = init[3];
    last[18] = init[2];

    idx_t i;
    for (i = 21; i <= n; ++i) {
        seq_t next = 0;
        next += last[0];                                       /* +1 * a(n-1) */
        next += (seq_t)4 * last[1];                            /* +4 * a(n-2) */
        next += (seq_t)8 * last[2];                            /* +8 * a(n-3) */
        next -= (seq_t)10 * last[3];                           /* -10 * a(n-4) */
        next -= (seq_t)26 * last[4];                           /* -26 * a(n-5) */
        next -= (seq_t)6 * last[5];                            /* -6 * a(n-6) */
        next += (seq_t)23 * last[6];                           /* +23 * a(n-7) */
        next += (seq_t)19 * last[7];                           /* +19 * a(n-8) */
        next -= (seq_t)33 * last[8];                           /* -33 * a(n-9) */
        next += (seq_t)42 * last[9];                           /* +42 * a(n-10) */
        next += (seq_t)54 * last[10];                          /* +54 * a(n-11) */
        next -= (seq_t)56 * last[11];                          /* -56 * a(n-12) */
        next -= (seq_t)61 * last[12];                          /* -61 * a(n-13) */
        next += (seq_t)24 * last[13];                          /* +24 * a(n-14) */
        next += (seq_t)29 * last[14];                          /* +29 * a(n-15) */
        next -= (seq_t)4 * last[15];                           /* -4 * a(n-16) */
        next += last[16];                                      /* +1 * a(n-17) */
        next -= last[17];                                      /* -1 * a(n-18) */
        next -= last[18];                                      /* -1 * a(n-19) */

        /* Shift the array to drop a(i-19) and add the new value */
        for (idx_t j = 18; j > 0; --j)
            last[j] = last[j - 1];
        last[0] = next;
    }

    return last[0]; /* a(n) */
}
