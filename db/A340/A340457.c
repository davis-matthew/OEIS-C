/*
 * A340457  Squares in A067459.
 * https://oeis.org/A340457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref, more
 * Formula:   a(n) = n-th term (in order of index m) of A067459(m) that is a
 *            perfect square, where A067459(m) = Sum_{k=1..m-1} (m^2 mod k^2);
 *            computed by direct search
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-10)
 * Helpers:   is_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_square.h"

static seq_t A067459_a(idx_t n)
{
    if (n <= 1) {
        return 0;
    }
    seq_t result = 0;
    seq_t n2 = (seq_t)n * (seq_t)n;
    for (idx_t k = 1; k < n; ++k) {
        seq_t k2 = (seq_t)k * (seq_t)k;
        result += n2 % k2;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1; m <= 20000; ++m) {
        seq_t v = A067459_a(m);
        if (is_square(v)) {
            ++count;
            if (count == n) {
                return v;
            }
        }
    }
    return 0;
}
