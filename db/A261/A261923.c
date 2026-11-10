/*
 * A261923  Number of steps to reach 0, starting at n, and iteration the map x
 *          -> A261922(x).
 * https://oeis.org/A261923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x -> A261922(x) needed to reach 0
 *            starting from n (a(0) = 0); A261922(x) = least m >= 0 whose
 *            binary string is not a substring of binary x. Computed by direct
 *            iteration
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9405 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A261923_step(uint64_t x)
{
    unsigned len = 0;
    uint64_t t = x;
    while (t) { len++; t >>= 1; }
    if (len == 0) len = 1;
    for (uint64_t m = 0; ; ++m) {
        int found = 0;
        if (m == 0) {
            if (x == 0) found = 1;
            else {
                for (unsigned s = 0; s < len; ++s)
                    if (((x >> s) & 1) == 0) { found = 1; break; }
            }
        } else {
            unsigned lm = 0;
            uint64_t u = m;
            while (u) { lm++; u >>= 1; }
            if (lm <= len) {
                uint64_t mask = (lm >= 64) ? ~(uint64_t)0 : (((uint64_t)1 << lm) - 1);
                for (unsigned s = 0; s + lm <= len; ++s) {
                    if (((x >> s) & mask) == m) { found = 1; break; }
                }
            }
        }
        if (!found) return m;
    }
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    uint64_t steps = 0;
    while (x != 0) {
        x = A261923_step(x);
        steps++;
    }
    return (seq_t)steps;
}
