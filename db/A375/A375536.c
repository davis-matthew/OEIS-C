/*
 * A375536  The maximum exponent in the prime factorization of the largest
 *          5-smooth divisor of n.
 * https://oeis.org/A375536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max{v₂(n), v₃(n), v₅(n)} – the maximum exponent among 2,
 *            3, 5 in the prime factorization of the largest 5‑smooth divisor
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Strip powers of 2, 3 and 5 from n and count them. */
    idx_t m = n;
    seq_t e2 = 0, e3 = 0, e5 = 0;

    /* Count 2's exponent (v₂). */
    while ((m & 1ULL) == 0ULL) {
        e2++;
        m >>= 1;
    }

    /* Count 3's exponent (v₃). */
    while (m % 3ULL == 0ULL) {
        e3++;
        m /= 3ULL;
    }

    /* Count 5's exponent (v₅). */
    while (m % 5ULL == 0ULL) {
        e5++;
        m /= 5ULL;
    }

    /* The answer is the largest of the three exponents. */
    seq_t max_exp = e2;
    if (e3 > max_exp) max_exp = e3;
    if (e5 > max_exp) max_exp = e5;

    return max_exp;
}
