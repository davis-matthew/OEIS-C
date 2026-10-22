/*
 * A370181  Size of the group Z_5*_/(Z_5*)^n, where Z_5 is the ring of 5-adic
 *          integers.
 * https://oeis.org/A370181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = (2 if n has exactly one factor 2, else 4 if n has at
 *            least two factors 2, else 1) * * 5^e where e is the exponent of
 *            5 in n
 * Range:     u8 n <= 499, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t tmp = n;
    idx_t e = 0, f = 0;

    /* exponent of 2 */
    while ((tmp & 1ULL) == 0) { /* divisible by 2 */
        ++e;
        tmp >>= 1;
    }

    /* remainder is odd; exponent of 5 */
    while (tmp % 5 == 0) {
        ++f;
        tmp /= 5;
    }

    seq_t result = 1;

    /* contribution of the factors of 2 */
    if (e == 1)
        result = 2;
    else if (e >= 2)
        result = 4;

    /* contribution of the factors of 5 */
    if (f > 0) {
        seq_t mult = 1;
        for (idx_t i = 0; i < f; ++i)
            mult *= 5;
        result *= mult;
    }

    return result;
}
