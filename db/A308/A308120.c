/*
 * A308120  Take all the integer-sided triangles with perimeter n and prime
 *          sides a, b, and c such that a <= b <= c. a(n) is the sum of all
 *          the b's.
 * https://oeis.org/A308120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the sum of the middle side b for all integer‑sided triangles
   with perimeter n and prime side lengths a ≤ b ≤ c.  The value is returned
   in the type seq_t.  The implementation enumerates all possible a and b and
   checks primeness with the provided utility function is_prime(idx_t). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* No prime triples exist for very small perimeters */
    if (n < 5) return (seq_t)0;

    seq_t sum = 0;
    idx_t max_a = n / 3;          /* because a ≤ b ≤ c and a + b + c = n */

    for (idx_t a = 2; a <= max_a; ++a) {
        if (!is_prime(a)) continue;           /* a must be prime */

        idx_t max_b = (n - a) / 2;            /* keep b ≤ c */

        for (idx_t b = a; b <= max_b; ++b) {
            if (!is_prime(b)) continue;       /* b must be prime */

            idx_t c = n - a - b;
            if (c < b) continue;             /* enforce b ≤ c */
            if (!is_prime(c)) continue;       /* c must be prime */

            /* Triangle inequality automatically satisfied because
               a + b > c ⇔ a + b > n/2, which holds for all b in the
               chosen range once a + b > n/2.  We check it explicitly
               for safety. */
            if (a + b <= n / 2) continue;

            sum += (seq_t)b;
        }
    }

    return sum;
}
