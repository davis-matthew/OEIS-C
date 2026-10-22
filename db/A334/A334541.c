/*
 * A334541  a(n) is the number of partitions of n into consecutive parts that
 *          differ by 5.
 * https://oeis.org/A334541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into consecutive parts that
 *            differ by 5
 * Range:     u8 n <= 2912, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t k_minus1 = k - 1;
        seq_t t = (seq_t)k * (seq_t)k_minus1 / 2;                /* k*(k-1)/2 */
        seq_t numerator = (seq_t)n + 5 * t;
        if ((numerator % (seq_t)k) == 0) {
            seq_t a_val = numerator / (seq_t)k;
            if (a_val >= 5 * (seq_t)k_minus1 + 1) {
                ++count;
            }
        }
        seq_t min_sum = (seq_t)k * ((5 * (seq_t)k - 3)) / 2;     /* k*(5k-3)/2 */
        if (min_sum > (seq_t)n) {
            break;
        }
    }
    return count;
}
