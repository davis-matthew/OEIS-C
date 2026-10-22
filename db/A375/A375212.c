/*
 * A375212  a(n) is the product of the leading base-3 digit of n and the sum
 *          of the squares of its base-3 digits.
 * https://oeis.org/A375212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = leading base‑3 digit of n × sum of squares of its base‑3
 *            digits
 * Range:     u8 n <= 8562, u16 n <= 8562, u32 n <= 8562, u64 n <= 8562,
 *            u128 n <= 8562, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n is guaranteed to be ≥ 1 */
    idx_t m = n;
    unsigned int leading = 0;
    unsigned int sumsq  = 0;

    /* Process digits from least significant to most significant.
       The most significant digit is the one obtained on the last
       iteration before the loop terminates. */
    while (m > 0) {
        unsigned int digit = m % 3;
        sumsq += digit * digit;
        m /= 3;
        if (m == 0) {
            leading = digit; /* most significant digit */
        }
    }

    return (seq_t)leading * (seq_t)sumsq;
}
