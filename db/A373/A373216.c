/*
 * A373216  Expansion of Sum_{k>=0} x^(6^k) / (1 - x^(6^k)).
 * https://oeis.org/A373216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min(v₂(n), v₃(n)) + 1 where v₂(n) and v₃(n) are the
 *            exponents of 2 and 3 in the prime factorization of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* n ≥ 1 is required by the sequence definition */
    idx_t m = n;
    seq_t v2 = 0, v3 = 0;

    /* Count powers of 2 dividing n */
    while ((m & 1) == 0) {
        v2++;
        m >>= 1;
    }

    /* Count powers of 3 dividing the remaining part */
    while (m % 3 == 0) {
        v3++;
        m /= 3;
    }

    /* Return min(v2, v3) + 1 */
    return (v2 < v3) ? v2 + 1 : v3 + 1;
}
