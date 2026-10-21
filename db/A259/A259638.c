/*
 * A259638  Number of (n+2)X(4+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000000 or 00000001.
 * https://oeis.org/A259638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 16*a(n-3) + 7*a(n-4) - 14*a(n-5) -
 *            45*a(n-6) - 14*a(n-7) + 5*a(n-8) + 38*a(n-9) + 13*a(n-10) -
 *            6*a(n-11) - 6*a(n-12) for n > 13; a(1)=133, a(2)=365, a(3)=1483,
 *            a(4)=4146, a(5)=12348, a(6)=42524, a(7)=133078, a(8)=406770,
 *            a(9)=1313046, a(10)=4180198, a(11)=13059524, a(12...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for NULL */

seq_t a(idx_t n)
{
    /* Initial 13 terms, 1‑based indexing (a(0) unused) */
    static const seq_t init[14] = {
        (seq_t)0,          /* unused index 0 */
        (seq_t)133,        /* a(1) */
        (seq_t)365,        /* a(2) */
        (seq_t)1483,       /* a(3) */
        (seq_t)4146,       /* a(4) */
        (seq_t)12348,      /* a(5) */
        (seq_t)42524,      /* a(6) */
        (seq_t)133078,     /* a(7) */
        (seq_t)406770,     /* a(8) */
        (seq_t)1313046,    /* a(9) */
        (seq_t)4180198,    /* a(10) */
        (seq_t)13059524,   /* a(11) */
        (seq_t)41407226,   /* a(12) */
        (seq_t)131557812   /* a(13) */
    };

    if (n <= 13) {
        /* Return pre‑computed value for small n */
        return init[(size_t)n];
    }

    /* Use a sliding window of the last 13 values:
       v[0] = a(i-13), v[1] = a(i-12), ..., v[12] = a(i-1) */
    seq_t v[13];
    for (size_t i = 0; i < 13; ++i) {
        v[i] = init[i + 1];
    }

    for (idx_t i = 14; i <= n; ++i) {
        seq_t next = v[12]                             /* a(i-1) */
                   + v[11] * (seq_t)2                  /* +2*a(i-2) */
                   + v[10] * (seq_t)16                /* +16*a(i-3) */
                   + v[9]  * (seq_t)7                 /* +7*a(i-4) */
                   - v[8]  * (seq_t)14                /* -14*a(i-5) */
                   - v[7]  * (seq_t)45                /* -45*a(i-6) */
                   - v[6]  * (seq_t)14                /* -14*a(i-7) */
                   + v[5]  * (seq_t)5                 /* +5*a(i-8) */
                   + v[4]  * (seq_t)38                /* +38*a(i-9) */
                   + v[3]  * (seq_t)13                /* +13*a(i-10) */
                   - v[2]  * (seq_t)6                 /* -6*a(i-11) */
                   - v[1]  * (seq_t)6;                /* -6*a(i-12) */

        /* Shift window left by one and insert the new value */
        for (size_t j = 0; j < 12; ++j) {
            v[j] = v[j + 1];
        }
        v[12] = next;
    }

    /* After the loop, v[12] holds a(n) */
    return v[12];
}
