/*
 * A390382  Feedback vertex set (decycling) number of the n-hypercube graph.
 * https://oeis.org/A390382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(0)=0, a(1)=0, a(2)=1, a(3)=3, a(4)=6; for n≥5: a(n) = 2^{n−1}
 *            − 2^{n−4} (i.e. 7·2^{n−4})
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 5) {
        static const seq_t base[] = { 0, 0, 1, 3, 6 };
        return base[n];
    }
    seq_t high = pow2(n - 1);  /* 2^(n−1) */
    seq_t low  = pow2(n - 4);  /* 2^(n−4) */
    return high - low;
}
