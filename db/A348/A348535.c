/*
 * A348535  Sum of all the parts in the partitions of n into 3 parts that
 *          divide the product of the other two parts within each partition.
 * https://oeis.org/A348535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum over all partitions of n into 3 positive parts
 *            a>=b>=c * of ( a if a | b*c, + b if b | a*c, + c if c | a*b ). *
 *            Each part contributes its value whenever it divides the product
 *            of the other two parts
 * Range:     u8 n <= 22, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 3) return (seq_t)0;

    seq_t sum = (seq_t)0;
    for (idx_t j = 1; j <= n / 3; ++j) {
        idx_t max_i = (n - j) / 2;
        for (idx_t i = j; i <= max_i; ++i) {
            idx_t k = n - i - j;          /* k >= i by construction */
            if (k < i) continue;          /* safety, though should not happen */

            seq_t prod_ik = (seq_t)i * (seq_t)k;   /* product of middle and largest part */
            if (prod_ik % (seq_t)j == 0) sum += (seq_t)j;

            seq_t prod_jk = (seq_t)j * (seq_t)k;   /* product of smallest and largest part */
            if (prod_jk % (seq_t)i == 0) sum += (seq_t)i;

            seq_t prod_ji = (seq_t)j * (seq_t)i;   /* product of smallest and middle part */
            if (prod_ji % (seq_t)k == 0) sum += (seq_t)k;
        }
    }
    return sum;
}
