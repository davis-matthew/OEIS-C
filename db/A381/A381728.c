/*
 * A381728  Number of minimum dominating sets in the n-Mycielski graph.
 * https://oeis.org/A381728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=1; a(2)=2; a(3)=5; for n>=4 a(n) = 60 * ∏_{k=5}^{n} (6*k^2
 *            - 38*k + 66) (n is the OEIS index). The sequence counts the
 *            number of minimum dominating sets in the n‑Mycielski graph
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;
    seq_t result = (seq_t)60;                 /* a(4) */
    if (n >= 5) {
        for (idx_t k = 5; k <= n; ++k) {
            seq_t kk = (seq_t)k;
            seq_t factor = 6 * kk * kk - 38 * kk + (seq_t)66;
            result *= factor;
        }
    }
    return result;
}
