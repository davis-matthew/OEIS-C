/*
 * A259729  Sum of digits of a(n) equals the sum of digits of 5*a(n).
 * https://oeis.org/A259729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th multiple of 9 such that digit_sum(x) ==
 *            digit_sum(5*x)
 * Range:     u8 n <= 6, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 0;
    for (;;) {
        seq_t cand = (seq_t)9 * (seq_t)k;
        if (digit_sum(cand) == digit_sum((seq_t)5 * cand)) {
            ++count;
            if (count == n) {
                return cand;
            }
        }
        ++k;
    }
}
