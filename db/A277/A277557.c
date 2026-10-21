/*
 * A277557  The ordered image of the 1-to-1 mapping of an integer ordered pair
 *          (x,y) into an integer using Cantor's pairing function, where 0 < x
 *          < y, gcd(x,y)=1 and x+y odd.
 * https://oeis.org/A277557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277557:
   a(n) is the n-th Cantor pairing value of ordered pairs (x,y) with
   0 < x < y, gcd(x,y)=1, and x+y odd.  The pairs are ordered
   by increasing Cantor image: for each odd sum s = x+y, y runs
   from floor(s/2)+1 up to s-1.  The mapping is f(x,y)=s(s+1)/2 + y. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of valid pairs seen so far   */
    for (idx_t s = 3; ; s += 2)  /* iterate over odd sums s >= 3          */
    {
        idx_t y_start = s / 2 + 1; /* first possible y for this s          */
        for (idx_t y = y_start; y < s; ++y)
        {
            idx_t x = s - y;         /* x < y, > 0 automatically            */
            if (gcd((seq_t)x, (seq_t)y) == 1)
            {
                ++count;
                if (count == n)
                {
                    seq_t result = ((seq_t)s * (s + 1)) / 2 + (seq_t)y;
                    return result;
                }
            }
        }
    }
}
