/*
 * A258034  Expansion of phi(q) * phi(q^9) in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A258034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (k,m) ∈ ℤ² with k² + 9·m² =
 *            n, * i.e. the coefficient of q^n in φ(q)·φ(q^9)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t sq_n = (seq_t)n;

    for (idx_t m = 0; ; ++m)
    {
        seq_t m2      = (seq_t)m * m;
        seq_t nine_m2 = (seq_t)9 * m2;
        if (nine_m2 > sq_n) break;

        seq_t r = sq_n - nine_m2;
        seq_t s = floor_sqrt(r);

        if (s * s == r)
        {
            if (m == 0)
                count += (s == 0) ? 1 : 2;
            else
                count += (s == 0) ? 2 : 4;
        }
    }
    return count;
}
