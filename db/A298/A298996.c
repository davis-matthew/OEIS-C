/*
 * A298996  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4 or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 5*a(n-1) + 6*a(n-2) + 7*a(n-3)
 *              -26*a(n-4) -53*a(n-5) -27*a(n-6) -39*a(n-7)
 *              +17*a(n-8) -5*a(n-9) +12*a(n-10)   (n > 11)
 *
 * Initial values (index 1..11):
 * a(1)=4, a(2)=31, a(3)=179, a(4)=1073, a(5)=6479,
 * a(6)=39015, a(7)=235033, a(8)=1416220, a(9)=8533123,
 * a(10)=51414948, a(11)=309794527
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial terms (index 0 unused) */
    static const seq_t init[12] = {
        0ULL,                      /* dummy */
        4ULL,                      /* a(1) */
        31ULL,                     /* a(2) */
        179ULL,                    /* a(3) */
        1073ULL,                   /* a(4) */
        6479ULL,                   /* a(5) */
        39015ULL,                  /* a(6) */
        235033ULL,                 /* a(7) */
        1416220ULL,                /* a(8) */
        8533123ULL,                /* a(9) */
        51414948ULL,               /* a(10) */
        309794527ULL               /* a(11) */
    };

    if (n <= 11)
        return init[n];

    /* Sliding window of the last 10 produced values:
     *   v[9] = a(n-1), v[8] = a(n-2), … , v[0] = a(n-10)   */
    seq_t v[10];
    v[9] = init[11];       /* a(11) */
    v[8] = init[10];       /* a(10) */
    v[7] = init[9];        /* a(9)  */
    v[6] = init[8];        /* a(8)  */
    v[5] = init[7];        /* a(7)  */
    v[4] = init[6];        /* a(6)  */
    v[3] = init[5];        /* a(5)  */
    v[2] = init[4];        /* a(4)  */
    v[1] = init[3];        /* a(3)  */
    v[0] = init[2];        /* a(2)  */

    for (idx_t current = 12; current <= n; ++current)
    {
        seq_t val = 0;

        /* Positive contributions */
        val += (seq_t)5  * v[9];
        val += (seq_t)6  * v[8];
        val += (seq_t)7  * v[7];
        val += (seq_t)17 * v[2];
        val += (seq_t)12 * v[0];

        /* Negative contributions */
        val -= (seq_t)26 * v[6];
        val -= (seq_t)53 * v[5];
        val -= (seq_t)27 * v[4];
        val -= (seq_t)39 * v[3];
        val -= (seq_t)5  * v[1];

        /* Shift window left: drop a(n-10), shift, then insert new value */
        for (int i = 0; i < 9; ++i)
            v[i] = v[i + 1];
        v[9] = val;
    }

    /* After loop, v[9] holds a(n) */
    return v[9];
}
