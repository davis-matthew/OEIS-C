/*
 * A331812  Number of integer solutions to x^3 + y^3 = n.
 * https://oeis.org/A331812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs of integers (x, y) such that x^3 + y^3 =
 *            n (ordered pairs)
 * Range:     u8 n <= 15, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   floor_cube_root, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine iterates over all integer x with |x| ≤ ⌊√n⌋,
   computes y^3 = n - x^3, and checks if y^3 is a perfect cube.
   The count of such x gives the required number of solutions. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* Limit for |x|: any larger |x| would make 3*x^2*d > n
       for integer d, thus no solutions. Use floor(sqrt(n)).   */
    idx_t limit = (idx_t)floor_sqrt((seq_t)n);

    seq_t count = 0;

    /* Loop over x from -limit to +limit inclusive. */
    for (int64_t x = -(int64_t)limit; x <= (int64_t)limit; ++x)
    {
        /* Compute x^3.  x is small enough that int64_t suffices for the tests
           (limit ≤ sqrt(n)). */
        int64_t cubeX = x * x * x;

        /* y^3 = n - x^3 */
        int64_t target = (int64_t)n - cubeX;

        /* Check if |target| is a perfect cube. */
        uint64_t absTarget = (target >= 0) ? (uint64_t)target : (uint64_t)(-target);
        seq_t rt = floor_cube_root((seq_t)absTarget);

        /* rt^3 == absTarget? */
        if (rt * rt * rt == (seq_t)absTarget)
        {
            /* Each valid x yields exactly one y, so increment the count. */
            count += 1;
        }
    }

    return count;
}
