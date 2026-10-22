/*
 * A333815  G.f.: Sum_{k>=1} x^(k*(3*k - 1)/2) / (1 - x^(3*k)).
 * https://oeis.org/A333815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to n = k*(3*k-1)/2 + 3*k*m with
 *            integers k ≥ 1, m ≥ 0. This equals the coefficient of xⁿ in
 *            Sum_{k≥1} x^(k*(3*k-1)/2)/(1 - x^(3*k))
 * Range:     u8 n <= 1033, u16 n <= 9930, u32 n <= 9930, u64 n <= 9930,
 *            u128 n <= 9930, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t base = (seq_t)k * (((seq_t)k) * 3 - 1) / 2;
        if ((seq_t)n < base) break;
        idx_t step = 3 * k;
        if ((n - (idx_t)base) % step == 0) {
            ++count;
        }
    }
    return count;
}
