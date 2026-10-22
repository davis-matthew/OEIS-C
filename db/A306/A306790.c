/*
 * A306790  Number of ways to write n as x*(3*x+1)/2 + y*(y+1)*(y+2)/2 +
 *          z*(z+1)*(z+2)/6, where x is a nonzero integer, and y and z are
 *          nonnegative integers.
 * https://oeis.org/A306790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as x*(3*x+1)/2 +
 *            y*(y+1)*(y+2)/2 + z*(z+1)*(z+2)/6, where x is a nonzero integer,
 *            and y,z are nonnegative integers
 * Range:     u8 n <= 1039, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   tetra_val, three_tetra
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/tetra_val.h"
#include "../utils/three_tetra.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    seq_t N = (seq_t)n;
    seq_t result = 0;

    /* Determine maximal z with T(z) <= N */
    idx_t max_z = 0;
    while (true)
    {
        seq_t t = tetra_val(max_z);
        if (t > N) break;
        max_z++;
    }
    if (max_z > 0) max_z--;   /* last valid index */

    for (idx_t k = 1; ; k++)
    {
        seq_t k_seq = (seq_t)k;
        seq_t p1 = k_seq * (3 * k_seq + 1) / 2;   /* x = k  */
        seq_t p2 = k_seq * (3 * k_seq - 1) / 2;   /* x = -k */

        bool any = false;

        if (p1 <= N)
        {
            any = true;
            seq_t m = N - p1;
            for (idx_t y = 0; ; y++)
            {
                seq_t py = three_tetra(y);
                if (py > m) break;
                seq_t rem = m - py;
                for (idx_t z = 0; z <= max_z; z++)
                {
                    seq_t tz = tetra_val(z);
                    if (tz == rem)
                    {
                        result++;
                        break;
                    }
                    if (tz > rem) break;
                }
            }
        }

        if (p2 <= N)
        {
            any = true;
            seq_t m = N - p2;
            for (idx_t y = 0; ; y++)
            {
                seq_t py = three_tetra(y);
                if (py > m) break;
                seq_t rem = m - py;
                for (idx_t z = 0; z <= max_z; z++)
                {
                    seq_t tz = tetra_val(z);
                    if (tz == rem)
                    {
                        result++;
                        break;
                    }
                    if (tz > rem) break;
                }
            }
        }

        if (!any) break;  /* both p1 and p2 exceed N */
    }

    return result;
}
