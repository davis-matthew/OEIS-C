/*
 * A392593  Numbers k such that A003415(k) == A276085(k) (mod 3), where
 *          A003415 is the arithmetic derivative and A276085 is the primorial
 *          base log-function.
 * https://oeis.org/A392593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A003415(k) == A276085(k) (mod 3);
 *            A276085(k) == e2 - e3 (mod 3); found by linear search
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 8245 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A392593_ok(uint64_t k) {
    uint64_t m = k, d = 0;
    int e2 = 0, e3 = 0;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            d += (uint64_t)(e % 3) * ((k / p) % 3);
            if (p == 2) e2 = e;
            else if (p == 3) e3 = e;
        }
    }
    if (m > 1) {
        d += (k / m) % 3;
        if (m == 2) e2 = 1;
        else if (m == 3) e3 = 1;
    }
    int r = (int)(d % 3);
    int s = ((e2 % 3) - (e3 % 3) + 3) % 3;
    return r == s;
}

seq_t a(idx_t n) {
    uint64_t count = 0, k = 0;
    while (count < (uint64_t)n) {
        k++;
        if (A392593_ok(k)) count++;
    }
    return (seq_t)k;
}
