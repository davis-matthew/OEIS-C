/*
 * A358008  Number of partitions of n into 8 distinct positive Fibonacci
 *          numbers (with a single type of 1).
 * https://oeis.org/A358008
 *
 * Offset:    87    (a(87) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 8 distinct positive
 *            Fibonacci numbers * (with a single type of 1). The values are
 *            computed by enumerating all 8‑subsets of the * set
 *            {1,2,3,5,8,13,21,34,55,89,144,233,377,610,987,1597,2584,4181,6765}
 *            * and counting their sums
 * Range:     u8 n <= 9736, u16 n <= 9736, u32 n <= 9736, u64 n <= 9736,
 *            u128 n <= 9736, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


seq_t a(idx_t n)
{
    /* Precomputation bounds and array */
    enum { MAX_N = 20000 };                      /* safe upper bound for all sums */
    static bool init = false;                    /* flag whether counts are ready */
    static seq_t counts[MAX_N + 1] = {0};         /* counts[sum] = number of ways */

    /* Fibonacci numbers (unique positive values) */
    static const idx_t fibs[19] =
        {1, 2, 3, 5, 8, 13, 21, 34, 55, 89,
         144, 233, 377, 610, 987, 1597, 2584, 4181, 6765};

    if (!init) {
        const int M = 19;                       /* number of available Fibonacci numbers */

        /* Enumerate all 8‑element subsets using 8 nested loops (C(19,8)=75582 combos) */
        for (int i0 = 0; i0 < M - 7; ++i0)
            for (int i1 = i0 + 1; i1 < M - 6; ++i1)
                for (int i2 = i1 + 1; i2 < M - 5; ++i2)
                    for (int i3 = i2 + 1; i3 < M - 4; ++i3)
                        for (int i4 = i3 + 1; i4 < M - 3; ++i4)
                            for (int i5 = i4 + 1; i5 < M - 2; ++i5)
                                for (int i6 = i5 + 1; i6 < M - 1; ++i6)
                                    for (int i7 = i6 + 1; i7 < M; ++i7) {
                                        idx_t sum =
                                            fibs[i0] + fibs[i1] + fibs[i2] + fibs[i3] +
                                            fibs[i4] + fibs[i5] + fibs[i6] + fibs[i7];
                                        if (sum <= MAX_N) {
                                            counts[sum] = counts[sum] + 1;
                                        }
                                    }

        init = true;
    }

    return (n <= MAX_N) ? counts[n] : 0;
}
