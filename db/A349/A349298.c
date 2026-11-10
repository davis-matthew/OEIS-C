/*
 * A349298  Positions k in row n of triangles S(n,k) = T(n,k) = 0, where
 *          A054521 = S and A349297 = T, or 0 if there are no such k.
 * https://oeis.org/A349298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = flattened rows; row r lists k in 1..r with gcd(r,k) > 1
 *            and not (r,k both even) and not (3|r and 3|k), or a single 0 if
 *            there is no such k. Computed by scanning rows
 * Range:     u8 n <= 1968, u16 n <= 10635, u32 n <= 10635, u64 n <= 10635,
 *            u128 n <= 10635, big any n
 * Validated: standard, pass, 92 terms through n = 9220 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A349298_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static int A349298_ok(uint64_t r, uint64_t k)
{
    if ((r & 1) == 0 && (k & 1) == 0) return 0;
    if (r % 3 == 0 && k % 3 == 0) return 0;
    return A349298_gcd(r, k) > 1;
}

seq_t a(idx_t n)
{
    uint64_t r = 0, cum = 0, target = (uint64_t)n;
    if (target == 0) return (seq_t)0;
    for (;;) {
        r++;
        uint64_t cnt = 0, k;
        for (k = 1; k <= r; k++)
            if (A349298_ok(r, k)) cnt++;
        uint64_t len = cnt ? cnt : 1;
        if (cum + len >= target) {
            if (cnt == 0) return (seq_t)0;
            uint64_t want = target - cum, seen = 0;
            for (k = 1; k <= r; k++) {
                if (A349298_ok(r, k)) {
                    seen++;
                    if (seen == want) return (seq_t)k;
                }
            }
            return (seq_t)0;
        }
        cum += len;
    }
}
