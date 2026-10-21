/*
 * A274496  Triangle read by rows: T(n,k) is the number of binary words of
 *          length n having degree of asymmetry equal to k (n >= 0; 0 <= k <=
 *          n/2).
 * https://oeis.org/A274496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattening of the triangle T(n,k) where T(n,k) =
 *            2^ceil(n/2) * binomial(floor(n/2), k). For a flat index i, we
 *            locate the row n such that the cumulative count S(n-1) ≤ i <
 *            S(n), then k = i - S(n-1) and return T(n,k)
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binom.h"

seq_t a(idx_t idx) {
    uint64_t i = (uint64_t)idx;
    uint64_t cum = 0;
    uint64_t n = 0;
    while (1) {
        uint64_t len = (n / 2) + 1;          /* number of terms in row n */
        if (cum + len > i) {                 /* row n contains the term */
            uint64_t k = i - cum;
            uint64_t floorHalf = n / 2;
            uint64_t ceilHalf  = (n + 1) / 2;
            seq_t pow_val = pow2((idx_t)ceilHalf);               /* 2^ceil(n/2) */
            seq_t bin_val = binom((idx_t)floorHalf, (idx_t)k);   /* binomial(...) */
            return pow_val * bin_val;
        }
        cum += len;
        ++n;
    }
}
