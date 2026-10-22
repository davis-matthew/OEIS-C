/*
 * A356629  a(n) = n! * Sum_{k=0..floor(n/3)} (n - 3*k)^k/(n - 3*k)!.
 * https://oeis.org/A356629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (n-3k)^k/(n-3k)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    idx_t max_k = n / 3;
    seq_t sum = 1;           /* term k = 0 contributes 1 */
    seq_t product = 1;       /* falling factorial product for current k (size 3k) */

    for (idx_t k = 1; k <= max_k; ++k) {
        /* Extend the falling factorial by the next three factors */
        seq_t f1 = (seq_t)(n - 3 * k + 1);
        seq_t f2 = (seq_t)(n - 3 * k + 2);
        seq_t f3 = (seq_t)(n - 3 * k + 3);
        product = product * f1 * f2 * f3;

        /* Base (n-3k) and its k-th power */
        seq_t base = (seq_t)(n - 3 * k);
        seq_t base_pow = big_pow(base, k);

        /* Term for this k */
        seq_t term = product * base_pow;
        sum += term;
    }

    return sum;
}
