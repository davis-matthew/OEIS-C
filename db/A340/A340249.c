/*
 * A340249  Sum of the largest parts t of the partitions of n into 4 parts
 *          q,r,s,t such that 1 <= q <= r <= s <= t and q + r + s > t.
 * https://oeis.org/A340249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum_{t=1}^{floor((n-1)/2)} t * p3(n-t, t)
 * where p3(N, t) is the number of nondecreasing solutions q≤r≤s≤t,
 * q+r+s = N, q,r,s ≥ 1.
 */
#include <stddef.h>
#include <stdint.h>
/* seq_t and idx_t are defined by the build system. */

seq_t a(idx_t n)
{
    /* Handle the offset: sequence starts at a(1). For n < 1, return 0. */
    if (n < 1)
        return 0;

    seq_t result = 0;

    /* t ranges from 1 to floor((n-1)/2) and satisfies q+r+s > t automatically. */
    idx_t max_t = (n - 1) / 2;
    for (idx_t t = 1; t <= max_t; ++t) {
        idx_t N = n - t;               /* sum of the three smaller parts */

        /* The three parts must be at least 1 each: N ≥ 3 */
        /* They also must not exceed t each: N ≤ 3*t */
        if (N < 3 || N > 3 * t)
            continue;

        /* Count partitions of N into 3 nondecreasing parts ≤ t. */
        idx_t count = 0;
        idx_t Nprime = N - 3;          /* now a' >= 0 where a = a'+1 */

        for (idx_t a = 0; a <= Nprime / 3; ++a) {    /* a'  */
            idx_t M = Nprime - 3 * a;                /* remaining after fixing a' */
            idx_t high = M / 2;                      /* maximum b with 2b ≤ M */

            /* Inequality a' + b + c ≤ t-1  =>  b ≥ a' + M - (t-1) */
            idx_t lower = a + M > (t - 1) ? a + M - (t - 1) : 0;
            if (lower <= high)
                count += high - lower + 1;
        }

        /* Each such partition contributes t to the sum. */
        result += (seq_t)count * (seq_t)t;
    }

    return result;
}
