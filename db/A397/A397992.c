/*
 * A397992  Antidiagonal sums of the array A397991.
 * https://oeis.org/A397992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n+2} (1 + floor(k*(n-k+2)/(k-1)))
 * Range:     u8 n <= 18, u16 n <= 355, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 2; k <= n + 2; ++k) {
        seq_t kseq = (seq_t)k;
        seq_t nseq = (seq_t)n;
        seq_t numerator   = kseq * (nseq - kseq + (seq_t)2);
        seq_t denominator = kseq - (seq_t)1;
        seq_t floor_val   = numerator / denominator;   /* integer truncation = floor */
        result += (seq_t)1 + floor_val;
    }
    return result;
}
