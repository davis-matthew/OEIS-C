/*
 * A341709  Write n as a sum of powers of 2 then replace each power of 2 in
 *          the sum by its decimal reversal A004094(n).
 * https://oeis.org/A341709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ rev_decimal(2^i) over all i such that the i‑th bit of n
 *            is 1
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Example: a(17) = rev_decimal(16)+rev_decimal(1) = 61+1 = 62. */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/rev_decimal.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t pos = 0;
    idx_t bits = n;
    while (bits) {
        if (bits & 1) {
            seq_t p = pow2(pos);
            sum += rev_decimal(p);
        }
        bits >>= 1;
        pos++;
    }
    return sum;
}
