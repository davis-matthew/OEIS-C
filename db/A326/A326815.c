/*
 * A326815  Dirichlet g.f.: zeta(s)^3 * Product_{p prime} (1 - 2 * p^(-s)).
 * https://oeis.org/A326815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = ∏_{p^e||n} (e+1)*(2-e)/2 (multiplicative)
 * Range:     i8 n <= 9832, i16 n <= 9832, i32 n <= 9832, i64 n <= 9832,
 *            i128 n <= 9832, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t result = 1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t ee = (seq_t)e;
            seq_t factor = ((ee + 1) * (2 - ee)) / 2;
            result *= factor;
        }
    }

    /* Remaining factor > 1 has exponent 1 → factor 1, so nothing to do. */

    return result;
}
