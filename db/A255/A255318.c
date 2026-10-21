/*
 * A255318  Expansion of psi(x^3) * f(x^2, x^4) in powers of x where psi(),
 *          f() are Ramanujan theta functions.
 * https://oeis.org/A255318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(m,k)∈ℕ×ℤ : 3*m*(m+1)/2 + 3*k*k - k = n}
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t sn = (seq_t)n;
    for (idx_t m = 0;; ++m) {
        seq_t tri3 = (seq_t)3 * (seq_t)m * (seq_t)(m + 1) / 2;
        if (tri3 > sn) break;
        seq_t rem = sn - tri3;
        seq_t delta = (seq_t)1 + (seq_t)12 * rem;
        seq_t s = floor_sqrt(delta);
        if (s * s == delta && (s % (seq_t)6 == 1 || s % (seq_t)6 == 5)) {
            ++count;
        }
    }
    return count;
}
