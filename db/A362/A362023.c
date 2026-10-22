/*
 * A362023  a(n) is the least positive integer whose decimal expansion is the
 *          concatenation of the decimal expansions of two numbers whose
 *          product is n.
 * https://oeis.org/A362023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min{ concat_parts(d, n/d), concat_parts(n/d, d) : d|n }
 * Range:     u8 n <= 25, u16 n <= 6662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    seq_t min_val = 0;
    bool first = true;

    for (idx_t d = 1; (__uint128_t)d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;

            seq_t cand1 = concat_parts(d, e);
            if (first || cand1 < min_val) {
                min_val = cand1;
                first = false;
            }

            seq_t cand2 = concat_parts(e, d);
            if (cand2 < min_val) {
                min_val = cand2;
            }
        }
    }

    return min_val;
}
