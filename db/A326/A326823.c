/*
 * A326823  a(n) is the number of iterations needed to reach 1 or 11 starting
 *          at n and using the map k -> (k/2 if k is even, otherwise k +
 *          (smallest square > k)). Set a(n) = -1 if the trajectory never
 *          reaches 1 or 11.
 * https://oeis.org/A326823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): number of iterations to reach 1 or 11 starting from n
 * using the map:
 *   k -> k/2        if k is even,
 *   k -> k + (s^2)  if k is odd, where s = smallest integer with s^2 > k.
 * The function returns 0 for n=1 or n=11; otherwise it counts steps until
 * either 1 or 11 is reached.  (The conjecture states the process always
 * terminates, so -1 is never returned.) */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t cur   = (seq_t)n;
    seq_t steps = 0;

    /* base cases: already at a goal state */
    if (cur == (seq_t)1 || cur == (seq_t)11)
        return steps;

    while (cur != (seq_t)1 && cur != (seq_t)11)
    {
        if ((cur & (seq_t)1) == 0)           /* even case */
        {
            cur >>= 1;                       /* k -> k/2 */
        }
        else                                 /* odd case */
        {
            seq_t s = floor_sqrt(cur);       /* s = floor(sqrt(cur)) */
            if (s * s <= cur)                /* ensure s^2 > cur */
                s += (seq_t)1;
            cur += s * s;                    /* k -> k + s^2 */
        }
        steps++;
    }

    return steps;
}
