/*
 * A327257  Positions of 0's in {A327256(n) : n > 0}.
 * https://oeis.org/A327257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A327256(k) = 0, i.e. floor(2*k*sqrt(8))
 *            = 2*floor(k*sqrt(8)); found by a linear search using exact
 *            integer comparisons
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 8385, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9874 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t A = 0;          /* floor(sqrt(8 k^2)) */
    idx_t count = 0;
    idx_t k = 0;
    for (;;)
    {
        k++;
        seq_t sk = (seq_t)k;
        seq_t m = (seq_t)8 * sk * sk;
        A += 2;           /* floor(k*sqrt8) grows by 2 or 3 per step */
        while ((A + 1) * (A + 1) <= m)
            A++;
        /* floor(sqrt(32k^2)) == 2A  <=>  (2A+1)^2 > 32 k^2 */
        seq_t t = (seq_t)2 * A + 1;
        if (t * t > (seq_t)4 * m)
        {
            count++;
            if (count == n)
                return (seq_t)k;
        }
    }
}
