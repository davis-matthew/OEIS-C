/*
 * A384715  a(n) = Sum_{k=0..n} (binomial(n, k) mod 4).
 * https://oeis.org/A384715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + number_of_10_patterns + (1 if the pattern '11'
 *            appears, otherwise 0)) * 2^popcount(n)
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    unsigned popcnt = 0;                 /* number of set bits */
    unsigned p10    = 0;                 /* count of '10' patterns */
    bool    has11   = false;             /* whether '11' occurs */

    /* Find the position of the highest set bit of n, or 0 if n==0 */
    idx_t mask = 0;
    for (int i = 63; i >= 0; --i) {
        if (n & ((idx_t)1 << i)) {
            mask = ((idx_t)1 << i);
            break;
        }
    }

    bool prev_bit = false;
    for (idx_t bit = mask; bit > 0; bit >>= 1) {
        bool cur_bit = (n & bit) != 0;
        if (cur_bit) {
            ++popcnt;
        }
        if (prev_bit && !cur_bit) {
            ++p10;
        }
        if (prev_bit && cur_bit) {
            has11 = true;
        }
        prev_bit = cur_bit;
    }

    /* For n == 0 the loop above is skipped; popcnt=0, p10=0, has11=false automatically. */

    seq_t mult   = (seq_t)(1 + p10 + (has11 ? 1 : 0));
    seq_t power  = pow2((idx_t)popcnt);
    return mult * power;
}
