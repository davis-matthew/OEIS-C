/*
 * A337730  a(n) = (4*n+3)! * Sum_{k=0..n} 1 / (4*k+3)!.
 * https://oeis.org/A337730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n+3)! * Σ_{k=0..n} 1/(4*k+3)!
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t p = (uint64_t)(4*i) * (4*i+1) * (4*i+2) * (4*i+3);
        res = res * (seq_t)p + (seq_t)1;
    }
    return res;
}
