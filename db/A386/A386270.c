/*
 * A386270  Total number of ones in runs of 1s of length <=2 over all binary
 *          strings of length n.
 * https://oeis.org/A386270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A386270: Total number of ones in runs of 1s of length ≤2 over
   all binary strings of length n
   Formula:
     a(0) = 0
     a(1) = 1
     a(2) = 4
     a(n) = n*2^(n-1)            for n < 3
     a(3) = 9
     a(n) = (6 + 4*n)*2^(n-4)    for n ≥ 4
*/
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Handle the offset n = 0 */
    if (n == 0) return 0;

    /* Small n < 3 : a(n) = n * 2^(n-1) */
    if (n == 1 || n == 2) {
        return (seq_t)n * pow2(n - 1);
    }

    /* n == 3 : a(3) = 9 (2^(−1) is not computable) */
    if (n == 3) {
        return (seq_t)9;
    }

    /* n >= 4 : a(n) = (6 + 4*n) * 2^(n-4) */
    return (seq_t)(6 + 4 * n) * pow2(n - 4);
}
