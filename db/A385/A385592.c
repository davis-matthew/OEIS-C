/*
 * A385592  Values of u in the quartets (2,u,v,w); i.e., values of u for
 *          solutions to 2*(2+u) = v*(v+w), in positive integers, with and
 *          v>m, sorted by nondecreasing values of u; see Comments.
 * https://oeis.org/A385592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating factor pairs v*t = 2u+4 with 2 < v < t
   and accumulating the corresponding u values in nondecreasing order. */
#include "../utils/floor_sqrt.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* out of range, not required but defensive */

    idx_t count = 0;          /* number of terms produced so far   */
    seq_t N = 6;              /* N = 2u + 4, start with u = 1      */

    for (;; N = N + 2)        /* iterate over even N >= 6           */
    {
        idx_t sqrt_N = (idx_t)floor_sqrt(N);   /* sqrt(N) as idx_t */

        for (idx_t d = 3; d <= sqrt_N; ++d)   /* iterate possible factors  */
        {
            seq_t d_seq = (seq_t)d;
            if ((N % d_seq) == (seq_t)0)      /* d divides N               */
            {
                seq_t other = N / d_seq;     /* t = N/d                     */
                if (other > d_seq)           /* enforce v < t              */
                {
                    seq_t u = (N - 4) / 2;   /* recover u from N           */
                    ++count;
                    if (count == n)
                        return u;
                }
            }
        }
    }
}
