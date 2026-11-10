/*
 * A327181  Positions of 0's in {A327180(n) : n > 0}.
 * https://oeis.org/A327181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with floor((2k+1)*sqrt(3)) -
 *            floor((k+1)*sqrt(3)) - floor(k*sqrt(3)) = 0; found by a linear
 *            search with incremental floor(j*sqrt(3))
 * Range:     u8 n <= 35, u16 n <= 9628, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9774 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Given m = floor(j*sqrt(3)), return floor((j+1)*sqrt(3)); the step is 1 or 2. */
static seq_t A327181_step(seq_t m, seq_t j)
{
    seq_t j1 = j + 1;
    seq_t t = m + 2;
    if (t * t <= (seq_t)3 * j1 * j1)
        return m + 2;
    return m + 1;
}

seq_t a(idx_t n)
{
    seq_t fk = 0, fk1 = 1, f2 = 1, j2 = 1;
    seq_t k = 0;
    idx_t count = 0;
    while (1)
    {
        k++;
        fk = fk1;
        fk1 = A327181_step(fk, k);
        f2 = A327181_step(f2, j2);
        j2++;
        f2 = A327181_step(f2, j2);
        j2++;
        if (f2 == fk1 + fk)
        {
            count++;
            if (count == n)
                return k;
        }
    }
}
