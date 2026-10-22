/*
 * A325106  Number of divisible binary-containment pairs of positive integers
 *          up to n.
 * https://oeis.org/A325106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A325101(n) - n * where A325101(n) counts the pairs (i,j)
 *            with 1 ≤ i ≤ j ≤ n, * i divides j, and (i & j) == i (i.e., the
 *            set of 1‑bits of i * is a subset of that of j). We subtract n to
 *            exclude the * trivial pairs (i,i)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t j = 1; j <= n; ++j) {
        for (idx_t d = 1; d <= j / d; ++d) {
            if (j % d == 0) {
                idx_t i1 = d;
                if (((seq_t)i1 & (seq_t)j) == (seq_t)i1) {
                    total++;
                }
                idx_t i2 = j / d;
                if (i2 != i1) {
                    if (((seq_t)i2 & (seq_t)j) == (seq_t)i2) {
                        total++;
                    }
                }
            }
        }
    }
    return total - (seq_t)n;
}
