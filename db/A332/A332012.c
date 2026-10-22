/*
 * A332012  a(n) is the greatest square of the form floor(n/k) where k > 0.
 * https://oeis.org/A332012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{m^2 | ∃k≥1 such that floor(n/k)=m^2} Equivalent: a(n)
 *            = max{m^2 ≤ n | floor(n/m^2) > floor(n/(m^2+1))}
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* a(0) = 0 */

    idx_t s = (idx_t)floor_sqrt((seq_t)n);  /* largest integer ≤ √n */

    while (s >= 1) {
        seq_t sq = (seq_t)s * (seq_t)s;      /* square = m^2 */
        idx_t denom = (idx_t)sq + 1;         /* m^2 + 1 */

        /* There is an integer k with floor(n/k) = sq
           iff  floor(n/(m^2+1)) < floor(n/m^2) */
        if (n / denom < n / (idx_t)sq)
            return sq;                       /* largest such square found */

        --s;
    }
    return (seq_t)0;   /* should not reach here */
}
