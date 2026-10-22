/*
 * A350497  Sum of the largest parts in all the partitions of n into 3 parts
 *          whose largest part is greater than or equal to the product of the
 *          other two.
 * https://oeis.org/A350497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 208, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Direct summation of the largest parts in all partitions of n into 3 parts
 * such that the largest part is at least the product of the other two.
 * For a partition n = i + j + k with 1 ≤ i ≤ j ≤ k,
 * include k in the sum iff k ≥ i * j.
 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_i = n / 3;          /* smallest part can be at most floor(n/3) */
    for (idx_t i = 1; i <= max_i; ++i) {
        idx_t max_j = (n - i) / 2;   /* middle part at most floor((n-i)/2) */
        for (idx_t j = i; j <= max_j; ++j) {
            idx_t k = n - i - j;     /* largest part */
            if (k >= i * j) {
                sum += (seq_t)k;     /* accumulate the largest part */
            }
        }
    }
    return sum;
}
