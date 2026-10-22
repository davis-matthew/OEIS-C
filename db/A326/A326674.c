/*
 * A326674  GCD of the set of positions of 1's in the reversed binary
 *          expansion of n.
 * https://oeis.org/A326674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = GCD of the set of positions of 1's in reversed binary
 *            expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t _gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n) {
    idx_t val = n;
    idx_t gcd_val = 0;
    idx_t bit_pos = 1;
    while (val != 0) {
        if (val & 1ULL) {
            if (gcd_val == 0) {
                gcd_val = bit_pos;
            } else {
                gcd_val = _gcd_idx(gcd_val, bit_pos);
            }
        }
        val >>= 1;
        ++bit_pos;
    }
    return (seq_t)gcd_val;
}
