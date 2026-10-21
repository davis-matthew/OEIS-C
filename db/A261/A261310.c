/*
 * A261310  a(n+1) = abs(a(n) - gcd(a(n), 10n+9)), a(1) = 1.
 * https://oeis.org/A261310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = |a(n) – gcd(a(n), 10n+9)|, for n ≥ 1
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n < 1) return 0; /* out of defined range, but not expected */

    seq_t prev = 1;          /* a(1) */
    if (n == 1) return prev; /* early return for the first term */

    for (idx_t i = 1; i < n; ++i) { /* i corresponds to the current n in the recurrence */
        seq_t v = (seq_t)(10 * i + 9);
        seq_t g = gcd(prev, v);
        prev = (prev >= g) ? (prev - g) : (g - prev); /* absolute difference */
    }

    return prev; /* a(n) */
}
