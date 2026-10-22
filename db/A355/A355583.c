/*
 * A355583  a(n) is the number of the 5-smooth divisors of n.
 * https://oeis.org/A355583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = (e2+1)·(e3+1)·(e5+1) where e2, e3, e5 are the exponents
 *            of 2, 3 and 5 in the prime factorisation of n. Divisors whose
 *            prime factors are only 2, 3 or 5 are called 5‑smooth. The
 *            function returns their count
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n;
    unsigned exp2 = 0, exp3 = 0, exp5 = 0;

    /* factor out powers of 2 */
    while ((tmp & 1) == 0) {
        exp2++;
        tmp >>= 1;
    }

    /* factor out powers of 3 */
    while (tmp % 3 == 0) {
        exp3++;
        tmp /= 3;
    }

    /* factor out powers of 5 */
    while (tmp % 5 == 0) {
        exp5++;
        tmp /= 5;
    }

    return ((seq_t)(exp2 + 1)) * ((seq_t)(exp3 + 1)) * ((seq_t)(exp5 + 1));
}
