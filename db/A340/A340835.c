/*
 * A340835  a(n) is the least k such that the digit reversal of k is greater
 *          than or equal to n.
 * https://oeis.org/A340835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k such that the decimal reversal of k is >= n.
 *            Since a(n) <= n+1 with equality iff n = 10^k (k>0), iterating k
 *            from 0 to n+1 is sufficient
 * Range:     u8 n <= 991, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/rev.h"

/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    /* The range [0 , n+1] always contains the answer. */
    for (idx_t k = 0; k <= n + 1; ++k) {
        if (rev(k) >= n)
            return (seq_t)k;
    }
    /* By construction this point is unreachable. */
    return 0;
}
