/*
 * A352263  Integers that need 5 iterations of the map x->A352172(x) to reach
 *          1.
 * https://oeis.org/A352263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer m such that exactly 5 iterations of x ->
 *            A352172(x) (product of cubes of nonzero digits) take m to 1;
 *            computed by direct search
 * Range:     u8 none, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* A352172 on native integers; returns 0 as a sentinel if the value exceeds ~1e18 */
static unsigned long long A352263_step(unsigned long long x) {
    unsigned long long prod = 1;
    while (x > 0) {
        unsigned long long d = x % 10;
        if (d != 0) {
            unsigned long long c = d * d * d;
            if (prod > 1000000000000000000ULL / c) return 0;
            prod *= c;
        }
        x /= 10;
    }
    return prod;
}

static int A352263_ok(unsigned long long x) {
    int steps = 0;
    while (x != 1) {
        if (steps >= 5) return 0;
        x = A352263_step(x);
        if (x == 0) return 0;
        steps++;
    }
    return steps == 5;
}

seq_t a(idx_t n) {
    unsigned long long x = 0;
    idx_t count = 0;
    while (count < n) {
        x++;
        if (A352263_ok(x)) count++;
    }
    return (seq_t)x;
}
