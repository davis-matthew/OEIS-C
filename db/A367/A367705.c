/*
 * A367705  Coefficients of expansion of (1 + 5*x + 11*x^2 + 5*x^3 + 7*x^4 +
 *          x^5)/(1 - x^3)^2 in powers of x.
 * https://oeis.org/A367705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n%3==0) ? 6*(n/3)+1 : (n%3==1) ? 12*(n/3)+5 :
 *            12*(n/3)+11;
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t k = n / 3;          /* quotient of n divided by 3 */
    switch (n % 3) {
        case 0:
            return 6 * (seq_t)k + 1;
        case 1:
            return 12 * (seq_t)k + 5;
        case 2:
            return 12 * (seq_t)k + 11;
    }
    /* unreachable, but keep compiler happy */
    return 0;
}
