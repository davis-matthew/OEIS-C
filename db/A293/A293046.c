/*
 * A293046  Number of even permutations on {1,2,...,n} with exactly 2 weak
 *          excedances.
 * https://oeis.org/A293046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 , n = 0 a(n) = 2^(n‑1) – 1 , n > 0 even a(n) = 2^(n‑1)
 *            – n , n > 0 odd Equivalent to the formulas in OEIS
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    /* 2^(n-1) */
    seq_t p = pow2((idx_t)(n - 1));
    if ((n & 1U) == 0) {          /* even n */
        return p - 1;
    } else {                     /* odd n */
        return p - (seq_t)n;
    }
}
