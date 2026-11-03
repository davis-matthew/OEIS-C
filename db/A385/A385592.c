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
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating factor pairs v*t = 2u+4 with 2 < v < t
   and accumulating the corresponding u values in nondecreasing order. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}
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
