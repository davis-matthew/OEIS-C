/*
 * A317453  Number of nX3 0..1 arrays with every element unequal to 1, 2, 4,
 *          5, 6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A317453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) =
 *            a(n-1)+4*a(n-2)+16*a(n-3)-11*a(n-4)-59*a(n-5)-99*a(n-6)
 *            +30*a(n-7)+252*a(n-8)+272*a(n-9)+45*a(n-10)-323*a(n-11)
 *            -234*a(n-12)-12*a(n-13)+50*a(n-14)-117*a(n-15)-84*a(n-16)
 *            -38*a(n-17)-4*a(n-18) for n > 20; initial values: a(1)=1,
 *            a(2)=11, a(3)=17, a(4)=36...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Initial 20 terms of the sequence */
    static const seq_t init[20] = {
        1LL,        11LL,       17LL,       36LL,       131LL,
        322LL,      762LL,      2337LL,     6165LL,     15869LL,
        44914LL,    121947LL,   324214LL,   894468LL,   2441451LL,
        6589367LL,  18023035LL, 49207556LL, 133636847LL, 364592196LL
    };

    if (n <= 20)
        return init[n - 1];

    /* Use a sliding window of the last 18 computed values.
       prev[0] = a(k-18), ..., prev[17] = a(k-1)   (for the current k) */
    seq_t prev[18];
    for (int i = 0; i < 18; ++i)
        prev[i] = init[i + 2];  /* For k = 21, prev holds a(3) .. a(20) */

    seq_t next;
    for (idx_t k = 21; k <= n; ++k) {
        next =  prev[17]                                /* a(k-1) */
             + (seq_t)4  * prev[16]                      /* 4*a(k-2) */
             + (seq_t)16 * prev[15]                      /* 16*a(k-3) */
             - (seq_t)11 * prev[14]                      /* -11*a(k-4) */
             - (seq_t)59 * prev[13]                      /* -59*a(k-5) */
             - (seq_t)99 * prev[12]                      /* -99*a(k-6) */
             + (seq_t)30 * prev[11]                      /* +30*a(k-7) */
             + (seq_t)252 * prev[10]                     /* +252*a(k-8) */
             + (seq_t)272 * prev[9]                      /* +272*a(k-9) */
             + (seq_t)45  * prev[8]                      /* +45*a(k-10) */
             - (seq_t)323 * prev[7]                      /* -323*a(k-11) */
             - (seq_t)234 * prev[6]                      /* -234*a(k-12) */
             - (seq_t)12  * prev[5]                      /* -12*a(k-13) */
             + (seq_t)50  * prev[4]                      /* +50*a(k-14) */
             - (seq_t)117 * prev[3]                      /* -117*a(k-15) */
             - (seq_t)84  * prev[2]                      /* -84*a(k-16) */
             - (seq_t)38  * prev[1]                      /* -38*a(k-17) */
             - (seq_t)4   * prev[0];                     /* -4*a(k-18) */

        if (k == n)
            return next;   /* final value */

        /* Shift the window for the next iteration */
        for (int i = 0; i < 17; ++i)
            prev[i] = prev[i + 1];
        prev[17] = next;
    }

    /* Never reached */
    return 0;
}
