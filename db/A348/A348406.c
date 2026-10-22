/*
 * A348406  Number of vertices on the axis of symmetry of the symmetric
 *          representation of sigma(n) with subparts.
 * https://oeis.org/A348406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + |{d : d divides n, sqrt(n/2) ≤ d < sqrt(2n)}|
 * Range:     u8 n <= 9165, u16 n <= 9165, u32 n <= 9165, u64 n <= 9165,
 *            u128 n <= 9165, big any n
 * Helpers:   middle_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/middle_divisor.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, n == 0 is unused */
    seq_t res = 1;                 /* account for the "+1" in the definition */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            if (middle_divisor(d1, n)) ++res;
            if (d1 != d2 && middle_divisor(d2, n)) ++res;
        }
    }
    return res;
}
