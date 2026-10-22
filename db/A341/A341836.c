/*
 * A341836  Dirichlet g.f.: 1 / zeta(s)^10.
 * https://oeis.org/A341836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative with a(p^e) = (-1)^e * binomial(10, e)
 * Range:     i8 n <= 11, i16 n <= 359, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplies the contribution of each prime power; if e > 10 the
   contribution is zero, thus the whole product is zero. */

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    static const seq_t bin[11] = {
        1,   10,  45, 120, 210,
        252, 210, 120, 45, 10,
        1
    };

    seq_t result = 1;
    idx_t temp   = n;

    /* handle factor 2 separately */
    if ((temp & 1) == 0) {
        idx_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        if (e > 10) return 0;
        seq_t term = bin[e];
        if (e & 1U) term = -term;
        result *= term;
    }

    /* process odd factors */
    for (idx_t p = 3; (idx_t)p * (idx_t)p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e > 10) return 0;
            seq_t term = bin[e];
            if (e & 1U) term = -term;
            result *= term;
        }
    }

    /* remaining prime factor, if any, has exponent 1 */
    if (temp > 1) {
        seq_t term = bin[1];
        /* exponent 1 is odd, so negate */
        term = -term;
        result *= term;
    }

    return result;
}
