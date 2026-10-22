/*
 * A338686  Number of ways to write n as x^5 + y^2 + [z^2/7], where x,y,z are
 *          integers with x >= 0, y >= 1 and z >= 2, and [.] is the floor
 *          function.
 * https://oeis.org/A338686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A338686
 * Formula: a(n) = #{(x,y,z) | n = x^5 + y^2 + ⌊z^2/7⌋,
 *                    x ≥ 0, y ≥ 1, z ≥ 2 }.
 * Implementation uses a simple exhaustive search bounded by the size
 * of n.  The complexity is O(n^{1/5}·n^{1/2}) for a given n, which
 * is easily fast enough for the required range (n ≲ 10 000).
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root of an unsigned 64‑bit integer.
 * Returns ⌊√x⌋.  Implementation uses a binary search that never
 * overflows because mid ≤ √x ≤ 2^32 when x fits in 64 bits. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Count the representations of n described above. */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1; behaviour for n<1 is irrelevant */
        return 0;

    seq_t total = 0;

    /* Loop over x: 0 ≤ x^5 ≤ n.  Compute x^5 safely with 128‑bit
     * intermediate to avoid overflow. */
    for (idx_t x = 0; ; ++x)
    {
        idx_t x5 = 1;
        for (int i = 0; i < 5; ++i)
        {
            unsigned __int128 tmp = (unsigned __int128)x5 * x;
            if (tmp > n) { x5 = n + 1; break; }
            x5 = (idx_t)tmp;
        }
        if (x5 > n) break;            /* no further x can satisfy */

        idx_t rest = n - x5;          /* remaining part to be written as y^2 + ⌊z^2/7⌋ */

        /* Loop over y: 1 ≤ y^2 ≤ rest.  Compute y^2 safely. */
        for (idx_t y = 1; ; ++y)
        {
            unsigned __int128 y2_128 = (unsigned __int128)y * y;
            if (y2_128 > rest) break;
            idx_t y2 = (idx_t)y2_128;

            idx_t s = rest - y2;      /* required ⌊z^2/7⌋ value */

            /* Find all z ≥ 2 such that 7*s ≤ z^2 < 7*(s+1). */
            unsigned __int128 left128  = (unsigned __int128)7 * s;
            unsigned __int128 right128 = left128 + 6;  /* inclusive upper bound */

            /* If the bounds exceed 64 bits, we truncate to UINT64_MAX
               – for the problem range this never occurs.  */
            idx_t left  = (idx_t)left128;
            idx_t right = (idx_t)right128;

            idx_t min_z = isqrt_u64(left);
            if ((unsigned __int128)min_z * min_z < left) ++min_z;  /* ceil(sqrt(left)) */

            idx_t max_z = isqrt_u64(right);   /* floor(sqrt(right)) */

            if (max_z < min_z) continue;      /* no admissible z */
            if (max_z < 2)   continue;        /* z must be ≥ 2 */
            if (min_z < 2)   min_z = 2;        /* enforce lower bound */

            if (max_z >= min_z)
                total += (seq_t)(max_z - min_z + 1);
        }
    }

    return total;
}
