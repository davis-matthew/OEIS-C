/*
 * A399945  Highest frequency of a digit (0, 1 or 2) in the ternary
 *          representation of n.
 * https://oeis.org/A399945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = max( number of 0's, number of 1's, number of 2's in
 *            base-3(n) )
 * Range:     u8 n <= 9724, u16 n <= 9724, u32 n <= 9724, u64 n <= 9724,
 *            u128 n <= 9724, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t cnt[3] = {0, 0, 0};
    idx_t m = n;
    while (m > 0) {
        unsigned int d = m % 3;
        cnt[d]++;          /* increment count of digit d */
        m /= 3;
    }
    seq_t max = cnt[0];
    if (cnt[1] > max) max = cnt[1];
    if (cnt[2] > max) max = cnt[2];
    return max;
}
