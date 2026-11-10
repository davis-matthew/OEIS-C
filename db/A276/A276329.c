/*
 * A276329  a(n) = largest k such that A001563(k) divides n.
 * https://oeis.org/A276329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k such that A001563(k) = k*k! divides n;
 *            A001563(1) = 1 so a(n) >= 1. Found by a direct loop over k
 * Range:     u8 n <= 4320, u16 n <= 4320, u32 n <= 4320, u64 n <= 4320,
 *            u128 n <= 4320, big any n
 * Validated: standard, pass, 136 terms through n = 3647 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t fact = 1;
    idx_t best = 1;
    for (idx_t k = 1;; k++) {
        fact *= k;
        if (fact > n / k) break;
        idx_t f = fact * k;
        if (f > n) break;
        if (n % f == 0) best = k;
    }
    return (seq_t)best;
}
