/*
 * A264184  Number of (n+1)X(2+1) arrays of permutations of 0..n*3+2 with each
 *          element having index change +-(.,.) 0,0 1,-1 or 2,2.
 * https://oeis.org/A264184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A264184: Number of (n+1)×3 arrays of permutations of 0..3 n+2
 * with each element having index‑change one of (0,0), (1,–1) or (2,2).
 *
 * The sequence satisfies the linear recurrence
 *
 *   a(n) =  4⋅a(n‑1) +   a(n‑2) –17⋅a(n‑4) +68⋅a(n‑5) +17⋅a(n‑6)
 *          +17⋅a(n‑8) –68⋅a(n‑9) –17⋅a(n‑10)+  a(n‑12)
 *          –4⋅a(n‑13) –  a(n‑14)      (n ≥ 15),
 *
 * with the 14 initial values (n = 1 … 14) shown in the OEIS entry.
 *
 * The implementation keeps a circular buffer of the last 15 terms
 * and evaluates the recurrence iteratively for any n ≥ 1.
 * It works for arbitrary unsigned `seq_t` (8–128 bits or big‑int)
 * and runs in O(n) time with a very small constant.
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial 14 values (index starts at 1).  Index 0 is unused. */
    static const seq_t init[15] = {
        0ULL,               /* dummy 0‑th element */
        4ULL, 24ULL, 81ULL, 375ULL, 1600ULL, 6656ULL,
        28561ULL, 120393ULL, 509796ULL, 2161720ULL,
        9150625ULL, 38773295ULL, 164249856ULL, 695734272ULL
    };

    if (n <= 14)
        return init[n];

    /* Circular buffer holding a(i) for the last 15 indices. */
    seq_t buf[15] = {0};
    for (idx_t i = 1; i <= 14; ++i)
        buf[i % 15] = init[i];

    for (idx_t i = 15; i <= n; ++i) {
        /* indices of needed terms in the circular buffer. */
        idx_t m1  = (i - 1) % 15;
        idx_t m2  = (i - 2) % 15;
        idx_t m4  = (i - 4) % 15;
        idx_t m5  = (i - 5) % 15;
        idx_t m6  = (i - 6) % 15;
        idx_t m8  = (i - 8) % 15;
        idx_t m9  = (i - 9) % 15;
        idx_t m10 = (i - 10) % 15;
        idx_t m12 = (i - 12) % 15;
        idx_t m13 = (i - 13) % 15;
        idx_t m14 = (i - 14) % 15;

        /* Compute positive and negative contributions separately to
         * avoid unsigned underflow. */
        seq_t pos = 4 * buf[m1] + buf[m2]
                  + 68 * buf[m5] + 17 * buf[m6]
                  + 17 * buf[m8] + buf[m12];

        seq_t neg = 17 * buf[m4] + 68 * buf[m9]
                  + 17 * buf[m10] + 4 * buf[m13]
                  + buf[m14];

        buf[i % 15] = pos - neg;   /* pos ≥ neg for all n ≥ 15   */
    }

    return buf[n % 15];
}
