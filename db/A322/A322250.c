/*
 * A322250  Take binary expansion of 2n-1 and delete the trailing block of
 *          1's, except if the number is 11...1, leave a single 1.
 * https://oeis.org/A322250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-1 if n is odd; else a(n/2). a(1) = 1
 * Range:     u8 n <= 256, u16 n <= 9802, u32 n <= 9802, u64 n <= 9802,
 *            u128 n <= 9802, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle offset: n >= 1 */
    if (n <= 1) {
        return (seq_t)1;
    }
    /* Reduce n by canceling trailing 0-bits (i.e. dividing by 2 while even) */
    while ((n & (idx_t)1) == 0) {
        n >>= 1;
    }
    /* If n has become 1, the sequence value is 1; otherwise n is odd and >1 */
    if (n == 1) {
        return (seq_t)1;
    }
    return (seq_t)(n - 1);
}
