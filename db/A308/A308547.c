/*
 * A308547  Number of ways to write n as a^2 + 2*b^2 + 2^c*3^d, where a,b,c,d
 *          are nonnegative integers.
 * https://oeis.org/A308547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) counts the number of representations of n as
 * a^2 + 2*b^2 + 2^c * 3^d with non‑negative integers a,b,c,d.
 * Each (a,b) that leaves a remainder of the form 2^c*3^d
 * corresponds to exactly one such representation.
 */
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t a = 0; (seq_t)a * a <= n; ++a) {
        seq_t a2 = (seq_t)a * a;
        idx_t rest1 = n - (idx_t)a2;
        for (idx_t b = 0; 2 * b * b <= rest1; ++b) {
            idx_t b2 = b * b;
            idx_t rest = rest1 - 2 * b2;
            if (rest == 0) continue;               // 2^c * 3^d > 0
            idx_t tmp = rest;
            while (tmp % 2 == 0) tmp /= 2;
            while (tmp % 3 == 0) tmp /= 3;
            if (tmp == 1) ++count;                 // valid 2^c * 3^d
        }
    }
    return count;
}
