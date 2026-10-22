/*
 * A349523  a(n) = Sum_{k=1..n} A339399(k).
 * https://oeis.org/A349523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A339399(k) where for each k: if k even : a_k
 *            = 1 + ⌊√(2k – 2)⌋ – ((k – 2·S)/2) if k odd : a_k = (k + 1 –
 *            2·S)/2 with S = Σ_{t=1..⌊√(2k – 3)⌋ – 1} ⌊(t + 1)/2⌋ for even k
 *            S = Σ_{t=1..⌊√(2k – 1)⌋ – 1} ⌊(t + 1)/2⌋ for odd k. The sum of
 *            these a_k gives the required sequence
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   _floor_isqrt, _sum_floor_half_plus
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_floor_isqrt.h"
#include "../utils/_sum_floor_half_plus.h"

seq_t a(idx_t n)
{
    if (n < 1ULL)
        return (seq_t)0;

    seq_t total = 0;

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t term;
        if ((i & 1ULL) == 0ULL)    /* even i */
        {
            idx_t sqrt1 = _floor_isqrt((idx_t)2 * i - 2);               /* ⌊√(2i – 2)⌋ */
            idx_t t = _floor_isqrt((idx_t)2 * i - 3) - 1;              /* ⌊√(2i – 3)⌋ – 1 */
            seq_t S = _sum_floor_half_plus(t);                         /* sum of floor((t+1)/2) */

            term = (seq_t)1 + (seq_t)sqrt1 - (( (seq_t)i - 2 * S ) / 2);
        }
        else                       /* odd i */
        {
            idx_t t = _floor_isqrt((idx_t)2 * i - 1) - 1;              /* ⌊√(2i – 1)⌋ – 1 */
            seq_t S = _sum_floor_half_plus(t);

            term = ((seq_t)i + 1 - 2 * S) / 2;
        }

        total += term;
    }

    return total;
}
