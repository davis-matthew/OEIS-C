/*
 * A335622  Number of integer-sided triangles with perimeter n such that the
 *          average of each pair of side lengths is prime.
 * https://oeis.org/A335622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count integer‑sided triangles with perimeter n such that
 *            (a+b)/2, (a+c)/2, and (b+c)/2 are all primes
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Triangle sides are a ≤ b ≤ c, a + b + c = n.  All sides must have
       the same parity as n, otherwise one of the averages is not an
       integer.  The inner loop runs over a and b subject to the triangle
       inequality (a + b > c). */
    seq_t count = 0;
    const idx_t parity = n & 1ULL;          /* desired parity of sides */

    for (idx_t a = 1; a <= n / 3; ++a) {
        if ((a & 1ULL) != parity) continue; /* a must match parity */

        /* b ranges from a up to (n - a)/2, guaranteeing b ≤ c. */
        const idx_t max_b = (n - a) / 2;
        for (idx_t b = a; b <= max_b; ++b) {
            if ((b & 1ULL) != parity) continue; /* b must match parity */

            /* Triangle inequality: a + b > c  ⇔  a + b > n/2. */
            if (a + b <= n / 2) continue;

            const idx_t c = n - a - b;

            /* All sums are even because of parity, so averages are integers. */
            const idx_t ab_avg = (a + b) >> 1;
            const idx_t ac_avg = (a + c) >> 1;
            const idx_t bc_avg = (b + c) >> 1;

            if (!is_prime(ab_avg) || !is_prime(ac_avg) || !is_prime(bc_avg))
                continue;

            ++count;
        }
    }

    return count;
}
