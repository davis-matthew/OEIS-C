/*
 * A270516  Number of ordered ways to write n = x^3*(x+1) + y*(y+1)/2 +
 *          z*(3z+2), where x and y are nonnegative integers, and z is an
 *          integer.
 * https://oeis.org/A270516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of ordered triples (x,y,z) with n = x^3*(x+1) +
 *            y*(y+1)/2 + z*(3z+2), where x,y are nonnegative integers and z
 *            is an integer
 * Range:     u8 n <= 4, u16 n <= 1180, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t nseq  = (seq_t)n;
    for (idx_t x = 0; ; ++x)
    {
        /* Compute X = x^3*(x+1) */
        seq_t X = (seq_t)x * (seq_t)x * (seq_t)x * ((seq_t)x + 1);
        if (X > nseq)
            break;                       /* further x only increase X */

        for (idx_t y = 0; ; ++y)
        {
            /* Compute Y = y*(y+1)/2 */
            seq_t Y = (seq_t)y * ((seq_t)y + 1) / 2;
            if (X + Y > nseq)
                break;                   /* further y only increase Y */

            seq_t r = nseq - X - Y;     /* residual that must equal z*(3z+2) */
            seq_t D = 4 + 12 * r;        /* discriminant 4+12r = (2+√D)^2 */

            seq_t sqrtD = floor_sqrt(D);
            if (sqrtD * sqrtD == D)     /* perfect square test */
            {
                uint32_t mod6 = (uint32_t)(sqrtD % (seq_t)6);
                if (mod6 == 2 || mod6 == 4)   /* guarantees integer z */
                    ++count;
            }
        }
    }
    return count;
}
