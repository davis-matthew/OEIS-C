/*
 * A243021  Numbers n such that n appears in the sequence beginning with the
 *          digit-sum of n and extended by adding successive digit-sums.
 * https://oeis.org/A243021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth integer n>9 that appears in the sequence defined
 *            by b(1)=digit_sum(n) and b(i)=b(i-1)+digit_sum(b(i-1))
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t cand = 10; ; ++cand) {
        seq_t cand_s = cand;
        seq_t x = digit_sum(cand_s);
        while (x < cand_s)
            x += digit_sum(x);
        if (x == cand_s) {
            ++found;
            if (found == n)
                return cand_s;
        }
    }
}
