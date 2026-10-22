/*
 * A392597  The largest 7-smooth divisor of n.
 * https://oeis.org/A392597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p∈{2,3,5,7}} p^{v_p(n)} (largest 7‑smooth divisor of
 *            n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    idx_t m = n;

    /* factor 2 */
    while ((m & 1) == 0) {
        res *= (seq_t)2;
        m >>= 1;
    }
    /* factor 3 */
    while (m % 3 == 0) {
        res *= (seq_t)3;
        m /= 3;
    }
    /* factor 5 */
    while (m % 5 == 0) {
        res *= (seq_t)5;
        m /= 5;
    }
    /* factor 7 */
    while (m % 7 == 0) {
        res *= (seq_t)7;
        m /= 7;
    }
    return res;
}
