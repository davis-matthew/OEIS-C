/*
 * A255022  Number of (n+2)X(3+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 0 or 1 and no antidiagonal sum 0 or 1 and no row sum 0 or 1
 *          and no column sum 0 or 1.
 * https://oeis.org/A255022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Empirical recurrence for A255022:
 * a(1)=117, a(2)=405, a(3)=1489, a(4)=5077, a(5)=17447,
 * a(6)=60237, a(7)=206739, a(8)=711741, a(9)=2455031,
 * a(10)=8451157, a(11)=29078117, a(12)=100127001,
 * a(13)=344830161, a(14)=1187280665, a(15)=4087598971,
 * a(16)=14073866371, a(17)=48459148689.
 * For n>17:
 * a(n)= a(n-1)+a(n-2)+12*a(n-3)+32*a(n-4)+44*a(n-5)+37*a(n-6)
 *      -32*a(n-7)-81*a(n-8)-12*a(n-9)+47*a(n-10)+38*a(n-11)+8*a(n-12)
 *      -6*a(n-13)-4*a(n-14)-4*a(n-15)
 */

#include <stddef.h>     /* for size_t */

static const seq_t init[17] = {
    (seq_t)117ULL,
    (seq_t)405ULL,
    (seq_t)1489ULL,
    (seq_t)5077ULL,
    (seq_t)17447ULL,
    (seq_t)60237ULL,
    (seq_t)206739ULL,
    (seq_t)711741ULL,
    (seq_t)2455031ULL,
    (seq_t)8451157ULL,
    (seq_t)29078117ULL,
    (seq_t)100127001ULL,
    (seq_t)344830161ULL,
    (seq_t)1187280665ULL,
    (seq_t)4087598971ULL,
    (seq_t)14073866371ULL,
    (seq_t)48459148689ULL
};

seq_t a(idx_t n)
{
    if (n == 0)              /* undefined, return 0 for safety */
        return (seq_t)0;

    if (n <= 17)              /* first 17 terms are known */
        return init[n - 1];

    /* buffer holds the last 15 terms needed for the recurrence:
     * last[0] = a(n-15), last[14] = a(n-1) at each iteration */
    seq_t last[15];
    /* initialise with a3 .. a17 (indices 2 .. 16) */
    for (int i = 0; i < 15; ++i)
        last[i] = init[i + 2];

    /* compute terms from n = 18 up to the requested n */
    for (idx_t k = 18; k <= n; ++k) {
        seq_t next =
            last[14] + last[13]                        /* a(k-1)+a(k-2) */
          + last[12] * (seq_t)12                       /* +12*a(k-3) */
          + last[11] * (seq_t)32                       /* +32*a(k-4) */
          + last[10] * (seq_t)44                       /* +44*a(k-5) */
          + last[9]  * (seq_t)37                       /* +37*a(k-6) */
          - last[8]  * (seq_t)32                       /* -32*a(k-7) */
          - last[7]  * (seq_t)81                       /* -81*a(k-8) */
          - last[6]  * (seq_t)12                       /* -12*a(k-9) */
          + last[5]  * (seq_t)47                       /* +47*a(k-10) */
          + last[4]  * (seq_t)38                       /* +38*a(k-11) */
          + last[3]  * (seq_t)8                        /* +8*a(k-12) */
          - last[2]  * (seq_t)6                        /* -6*a(k-13) */
          - last[1]  * (seq_t)4                        /* -4*a(k-14) */
          - last[0]  * (seq_t)4;                       /* -4*a(k-15) */

        /* shift left and insert the new term */
        for (int i = 0; i < 14; ++i)
            last[i] = last[i + 1];
        last[14] = next;
    }

    return last[14];
}
