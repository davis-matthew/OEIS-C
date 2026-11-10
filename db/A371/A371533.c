/*
 * A371533  Numbers k such that A001227(2*k) = A001222(2*k).
 * https://oeis.org/A371533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A001227(2*k) = A001222(2*k); found by a
 *            linear search over k
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A371533_ok(idx_t k) {
    idx_t m = k;
    idx_t omega = 1; /* Omega(2k) = 1 + Omega(k) */
    while (m % 2 == 0) {
        m /= 2;
        omega++;
    }
    idx_t divs = 1; /* number of odd divisors of k */
    for (idx_t d = 3; d <= m / d; d += 2) {
        idx_t e = 0;
        while (m % d == 0) {
            m /= d;
            e++;
        }
        if (e) {
            divs *= (e + 1);
            omega += e;
        }
    }
    if (m > 1) {
        divs *= 2;
        omega++;
    }
    return divs == omega;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        k++;
        if (A371533_ok(k)) count++;
    }
    return (seq_t)k;
}
