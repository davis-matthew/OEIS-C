/*
 * A241869  Number of compositions of n such that the smallest part has
 *          multiplicity nine.
 * https://oeis.org/A241869
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n in which the smallest part
 *            appears exactly nine times
 * Range:     u8 n <= 14, u16 n <= 20, u32 n <= 36, u64 n <= 71, u128 n <= 151,
 *            big any n
 * Helpers:   binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n < 9) return (seq_t)0;
    seq_t total = (seq_t)0;

    idx_t max_s = n / 9;
    for (idx_t s = 1; s <= max_s; ++s) {
        idx_t R = n - 9 * s;
        if (R == 0) {
            total += (seq_t)1;
            continue;
        }

        idx_t kmax = R / (s + 1);
        for (idx_t k = 1; k <= kmax; ++k) {
            idx_t n1 = R - k * s - 1;   /* n1 = R - k*s - 1 = Y + k - 1 */
            idx_t r1 = k - 1;           /* r1 = k-1   */
            seq_t bin1 = binom(n1, r1); /* C(R - k*s - 1, k-1) */
            seq_t bin2 = binom(k + 9, 9);/* C(k+9,9)   */
            seq_t prod = seq_mul(bin1, bin2);
            total += prod;
        }
    }
    return total;
}
