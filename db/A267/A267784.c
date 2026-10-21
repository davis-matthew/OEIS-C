/*
 * A267784  Number of nX4 0..1 arrays with every repeated value in every row
 *          greater than or equal to, and in every column greater than, the
 *          previous repeated value.
 * https://oeis.org/A267784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the recurrence a(n) =
 *            2*a(n-1)+6*a(n-2)-14*a(n-3)-14*a(n-4)+42*a(n-5)+14*a(n-6)
 *            -70*a(n-7)+70*a(n-9)-14*a(n-10)-42*a(n-11)+14*a(n-12)+14*a(n-13)
 *            -6*a(n-14)-2*a(n-15)+a(n-16) for n > 18 with initial values
 *            a(1..18) = 15,225,1056,5081,14956,44742,102954,
 *            238813,472174,935890,166925...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 34, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;      /* memoised sequence, index 1..n */
    static size_t cap = 0;         /* allocated capacity (index 0 unused) */
    static idx_t last = 0;         /* highest index computed */

    if (n < 1)
        return 0;                  /* out of defined range */

    /* Ensure room for indices up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq = tmp;
        cap = new_cap;
    }

    /* Compute missing terms if needed */
    if (last < n) {
        static const seq_t init[18] = {
            15ULL, 225ULL, 1056ULL, 5081ULL, 14956ULL, 44742ULL,
            102954ULL, 238813ULL, 472174ULL, 935890ULL, 1669256ULL,
            2973801ULL, 4921484ULL, 8116406ULL, 12686802ULL,
            19733869ULL, 29485150ULL, 43804354ULL
        };
        idx_t i;
        for (i = last + 1; i <= n; ++i) {
            if (i <= 18) {
                seq[(size_t)i] = init[i - 1];
            } else {
                seq_t val =
                    (seq_t)2  * seq[(size_t)i - 1] +
                    (seq_t)6  * seq[(size_t)i - 2] -
                    (seq_t)14 * seq[(size_t)i - 3] -
                    (seq_t)14 * seq[(size_t)i - 4] +
                    (seq_t)42 * seq[(size_t)i - 5] +
                    (seq_t)14 * seq[(size_t)i - 6] -
                    (seq_t)70 * seq[(size_t)i - 7] +
                    (seq_t)70 * seq[(size_t)i - 9] -
                    (seq_t)14 * seq[(size_t)i -10] -
                    (seq_t)42 * seq[(size_t)i -11] +
                    (seq_t)14 * seq[(size_t)i -12] +
                    (seq_t)14 * seq[(size_t)i -13] -
                    (seq_t)6  * seq[(size_t)i -14] -
                    (seq_t)2  * seq[(size_t)i -15] +
                    seq[(size_t)i -16];
                seq[(size_t)i] = val;
            }
        }
        last = n;
    }

    return seq[(size_t)n];
}
