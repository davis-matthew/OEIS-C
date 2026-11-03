/*
 * A256031  Number of irreducible idempotents in partial Brauer monoid PB_n.
 * https://oeis.org/A256031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   if n is odd, a(n) = 2*n!; if n is even, a(n) = (n+1)!/n * i.e.
 *            a(2m-1) = 2*(2m-1)! and a(2m) = (2m+1)*(2m-1)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute (2m-1)! as a seq_t.  m must be >= 1. */
static seq_t odd_factorial(idx_t m) {
    seq_t f = 1;
    for (idx_t k = 1; k < m; ++k) {
        f = f * (seq_t)(2 * k);
        f = f * (seq_t)(2 * k + 1);
    }
    return f;
}

seq_t a(idx_t n) {
    /* n >= 1 by requirement. */
    if (n & 1U) {                /* odd index: a(2m-1) */
        idx_t m = (n + 1) / 2;
        seq_t f = odd_factorial(m);
        return f * 2;
    } else {                    /* even index: a(2m) */
        idx_t m = n / 2;
        seq_t f = odd_factorial(m);
        return f * (seq_t)(2 * m + 1);
    }
}
