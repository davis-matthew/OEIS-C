/*
 * A341915  For any nonnegative number n with runs in binary expansion (r_1,
 *          ..., r_w), a(n) = Sum_{k = 1..w} 2^(r_1 + ... + r_k - 1).
 * https://oeis.org/A341915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = Σ_{k=1..w} 2^(r₁+⋯+r_k−1) where (r₁,…,r_w) are the run
 *            lengths of the binary representation of n (most‑significant bit
 *            first)
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    idx_t msb = floor_log2_uint64(n);
    seq_t result = (seq_t)0;
    idx_t cumulative = 0;
    idx_t run_len = 0;
    unsigned curr_bit = 0;

    for (idx_t i = msb; ; --i) {
        unsigned bit = (unsigned)((n >> i) & (idx_t)1);

        if (run_len == 0) {
            curr_bit = bit;
            run_len = 1;
        } else if (bit == curr_bit) {
            ++run_len;
        } else {
            cumulative += run_len;
            result += pow2(cumulative - 1);
            curr_bit = bit;
            run_len = 1;
        }

        if (i == 0) break;
    }

    cumulative += run_len;
    result += pow2(cumulative - 1);
    return result;
}
