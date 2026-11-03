/*
 * A291884  Complement of A039691.
 * https://oeis.org/A291884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that at least one adjacent pair of decimal digits
   has a sum of 10 or more (the complement of A039691). */
#include <stdbool.h>
#include <stdint.h>

static inline bool has_carry(idx_t x) {
    /* Return true if any adjacent digit pair of x sums to >=10. */
    while (x >= 10) {
        idx_t d1 = x % 10;
        x /= 10;
        idx_t d2 = x % 10;
        if (d1 + d2 >= 10)
            return true;
    }
    return false;
}

seq_t a(idx_t n) {
    /* Find the n-th integer that has at least one adjacent digit pair
       with sum >=10.  The search is linear and sufficient for n up to 10000. */
    idx_t count = 0;
    idx_t x = 1;
    while (true) {
        if (has_carry(x)) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
        ++x;
    }
}
