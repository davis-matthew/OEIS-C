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
 * Helpers:   odd_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/odd_factorial.h"

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
