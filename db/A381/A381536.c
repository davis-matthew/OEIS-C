/*
 * A381536  Number of labeled histories for rooted 4-furcating trees with 3n+1
 *          leaves if simultaneous 4-furcations are not allowed.
 * https://oeis.org/A381536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((3*n+1)!/24^n) * ∏_{i=1..n} (3*i-2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n≥0, a(n+1) = ((27*n^4+90*n^3+105*n^2+50*n+8)/8) * a(n) */

#include <stdint.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;

    for (idx_t k = 0; k < n; ++k)
    {
        /* compute factor ((27*k^4+90*k^3+105*k^2+50*k+8)/8) in 128-bit */
        unsigned __int128 kk   = k;
        unsigned __int128 num  = 27ULL * kk * kk * kk * kk
                               + 90ULL * kk * kk * kk
                               + 105ULL * kk * kk
                               + 50ULL * kk
                               + 8ULL;
        unsigned __int128 factor = num / 8ULL;

        result = seq_mul(result, (seq_t)factor);
    }

    return result;
}
