/*
 * A355582  a(n) is the largest 5-smooth divisor of n.
 * https://oeis.org/A355582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product of p^v_p(n) for all primes p ≤ 5 that divide n
 *            (the largest 5‑smooth divisor of n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    seq_t res = 1;

    /* extract all powers of 2 */
    while ((m & 1ULL) == 0) {
        res = res * (seq_t)2;
        m >>= 1;
    }

    /* extract powers of 3 */
    while (m % 3 == 0) {
        res = res * (seq_t)3;
        m /= 3;
    }

    /* extract powers of 5 */
    while (m % 5 == 0) {
        res = res * (seq_t)5;
        m /= 5;
    }

    return res;
}
