/*
 * A353827  Inverse permutation to A353826.
 * https://oeis.org/A353827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = inverse of A353826; keep the positions of the nonzero
 *            ternary digits of n, and replace the k-th rightmost nonzero
 *            digit c_k by c_k*c_{k-1} mod 3 (c_0 = 1)
 * Range:     u8 n <= 244, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Validated: standard, pass, 84 terms through n = 6208 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t prev = 1;
    seq_t p3 = 1;
    while (n) {
        unsigned digit = (unsigned)(n % 3);
        n /= 3;
        if (digit != 0) {
            unsigned d = (digit * (unsigned)prev) % 3;
            result += (seq_t)d * p3;
            prev = (seq_t)digit;
        }
        p3 *= 3;
    }
    return result;
}
