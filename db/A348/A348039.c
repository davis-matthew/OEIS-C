/*
 * A348039  a(n) = gcd(A003557(n), A327564(n)).
 * https://oeis.org/A348039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = gcd(A003557(n), A327564(n)), where A003557(n) = Product
 *            p^(e-1) and A327564(n) = Product (p+1)^(e-1) over n = Product
 *            p^e
 * Range:     u8 n <= 2591, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 124 terms through n = 9217 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t x = 1;  /* A003557 */
    seq_t y = 1;  /* A327564 */
    uint64_t t = (uint64_t)n;

    for (uint64_t p = 2; p <= t / p; p += (p == 2 ? 1 : 2)) {
        uint32_t e = 0;
        while (t % p == 0) {
            t /= p;
            e++;
        }
        for (uint32_t i = 1; i < e; i++) {
            x *= (seq_t)p;
            y *= (seq_t)(p + 1);
        }
    }
    /* remaining prime factor has exponent 1: contributes 1 to both */
    return gcd(x, y);
}
