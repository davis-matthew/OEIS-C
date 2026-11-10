/*
 * A356219  Erroneous version of: Intersection of A001952 and A003151.
 * https://oeis.org/A356219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = floor((n+117)*(1+sqrt(2))) = A003151(n+117) = (n+117) +
 *            floor(sqrt(2*(n+117)^2))
 * Range:     i8 none, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-10)
 * Helpers:   isqrt_u128_to_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u128_to_u64.h"

static seq_t A003151_a(idx_t n)
{
    unsigned __int128 nn = (unsigned __int128)n;
    uint64_t r = isqrt_u128_to_u64(2 * nn * nn);
    seq_t result = (seq_t)n;
    result += (seq_t)r;
    return result;
}

seq_t a(idx_t n)
{
    return A003151_a(n + 117);
}
