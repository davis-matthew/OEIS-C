/*
 * A326825  a(n) is the number of iterations needed to reach 1 or 5 starting
 *          at n and using the map k -> (k/2 if k is even, otherwise k +
 *          (smallest triangular number > k)). Set a(n) = -1 if the trajectory
 *          never reaches 1 or 5.
 * https://oeis.org/A326825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A326825:
 * a(n) = number of iterations needed to reach 1 or 5 starting at n
 * using the map:
 *    if k is even: k -> k/2
 *    if k is odd : k -> k + T   where T is the smallest triangular
 *                     number > k.
 * Returns -1 (as the maximum seq_t value) if the iteration never
 * reaches 1 or 5. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t x) is provided */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;              /* current value */
    seq_t steps = 0;                 /* number of iterations */

    /* safety limit to avoid infinite loops if the conjecture fails */
    const seq_t MAX_STEPS = (seq_t)1 << 31;   /* large enough for all HS tests */

    while (k != 1 && k != 5)
    {
        /* even case */
        if ((k & 1) == 0)
        {
            k /= 2;
        }
        else /* odd case */
        {
            /* compute smallest triangular number > k
             * T_m = m(m+1)/2 , find minimal m with T_m > k
             * m = ( floor_sqrt(1+8k) - 1 ) / 2 + 1
             */
            seq_t s = k * (seq_t)8 + 1;                 /* 1 + 8k */
            seq_t r = floor_sqrt(s);                   /* floor(sqrt()) */
            seq_t m = (r - (seq_t)1) / (seq_t)2 + 1;    /* minimal m */

            seq_t T = m * (m + (seq_t)1) / (seq_t)2;     /* triangular number */
            k += T;
        }

        ++steps;
        if (steps > MAX_STEPS)          /* not reached 1 or 5 */
            return (seq_t)-1;            /* returns maximum unsigned value */
    }
    return steps;
}
