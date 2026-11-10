/*
 * A355115  a(n) = index of n^2 in A355159.
 * https://oeis.org/A355115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = index of n^2 in A355159 (offset 0, starting
 *            0,1,3,4,5,9,...), i.e. the count of k < n^2 with frac(k^(3/2)) <
 *            1/2; computed by direct counting
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-10)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isqrt_seq.h"

static inline bool A355115_ok(seq_t k)
{
    seq_t k3 = k * k * k;
    seq_t m = isqrt_seq(k3);
    return k3 <= m * m + m;
}

seq_t a(idx_t n)
{
    seq_t limit = (seq_t)n * (seq_t)n;
    seq_t count = 0;
    for (seq_t k = 0; k < limit; ++k) {
        if (A355115_ok(k)) ++count;
    }
    return count;
}
