/*
 * A280321  Number of 2 X 2 matrices with all elements in {0,..,n} having
 *          determinant = n*permanent.
 * https://oeis.org/A280321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)^2 for n != 1; a(1) = 12
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 990, u64 n <= 990,
 *            u128 n <= 990, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)12;
    }
    seq_t temp = (seq_t)(2 * n + 1);
    return temp * temp;
}
