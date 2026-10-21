/*
 * A260804  Number of ways to write n as n = x * y * z * t + x + y + z + t
 *          where 1 <= x <= y <= z <= t <= n.
 * https://oeis.org/A260804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (x,y,z,t) | 1 ≤ x ≤ y ≤ z ≤ t ≤ n , n = x*y*z*t + x +
 *            y + z + t }
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine below enumerates all quadruples while pruning
   paths that cannot satisfy the equation, and counts the exact
   number of solutions.  It uses only integer arithmetic and
   works for any idx_t/seq_t type pair. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t x = 1; x <= n; ++x) {
        /* For any y with x+y>n, no further y can work. */
        for (idx_t y = x; y <= n; ++y) {
            if (x + y > n) break;          /* no room for z,t */

            /* Remaining sum after fixing x and y. */
            uint64_t nxy = (uint64_t)n - x - y;

            for (idx_t z = y; z <= n; ++z) {
                /* If z exceeds the available sum, stop. */
                if (z > nxy) break;

                uint64_t nxyz = nxy - z;

                /* The denominator 1 + x*y*z is positive and will
                   quickly outgrow the remaining sum.  If it is
                   larger than the sum, no t ≥ 1 can exist. */
                uint64_t denom = 1ULL + (uint64_t)x * y * z;
                if (denom > nxyz) break;

                /* t must be an integer. */
                if (nxyz % denom == 0) {
                    uint64_t t = nxyz / denom;

                    /* Enforce t ≥ z and t ≤ n. */
                    if (t >= z && t <= n) {
                        ++count;
                    }
                }
            }
        }
    }

    return count;
}
