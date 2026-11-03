/*
 * A280025  Expansion of phi_{7, 4}(x) where phi_{r, s}(x) = Sum_{n, m>0} m^r
 *          * n^s * x^{m*n}.
 * https://oeis.org/A280025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^4 * σ₃(n) where σ₃(n) = Σ_{d|n} d³ (sum of cubes of
 *            divisors)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 551, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 953 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

extern seq_t a(idx_t n);

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t result = 1;
    idx_t m = n;

    /* factor n by trial division */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            /* σ₃(p^e) = (p^{3(e+1)} - 1) / (p³ - 1) */
            seq_t numerator   = ipow_u64_idx(p, 3 * (e + 1)) - 1;
            seq_t denominator = ipow_u64_idx(p, 3) - 1;
            seq_t sigma3      = numerator / denominator;

            /* multiply by n^4 contribution: p^{4e} */
            seq_t pow_p4e = ipow_u64_idx(p, 4 * e);
            result *= sigma3 * pow_p4e;
        }
    }

    /* handle remaining prime factor (if any) */
    if (m > 1) {
        seq_t numerator   = ipow_u64_idx(m, 6) - 1;   /* e = 1 → 3*(e+1)=6 */
        seq_t denominator = ipow_u64_idx(m, 3) - 1;
        seq_t sigma3      = numerator / denominator;
        seq_t pow_p4e     = ipow_u64_idx(m, 4);        /* 4*e = 4 */
        result *= sigma3 * pow_p4e;
    }

    return result;
}
