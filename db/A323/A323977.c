/*
 * A323977  Expansion of determinant of 6 X 6 matrix M_{u,v} = q^(maj(u*v^-1))
 *          where u, v in S_3 and maj = MacMahon's major index (cf. A008302).
 * https://oeis.org/A323977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of q^n in (1-q^2)^3 * (1-q^3)^4
 * Range:     i8 n <= 18, i16 n <= 18, i32 n <= 18, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The product expands to a polynomial of degree 18,
       so coefficients beyond 18 are zero. */
    if (n > 18)
        return 0;

    /* Binomial coefficients for (1-q^2)^3 and (1-q^3)^4 */
    const seq_t c3[4] = {1, 3, 3, 1};
    const seq_t c4[5] = {1, 4, 6, 4, 1};

    seq_t total = 0;
    for (idx_t i = 0; i <= 3; ++i) {
        for (idx_t j = 0; j <= 4; ++j) {
            if (2 * i + 3 * j != n)
                continue;

            /* Overall sign determined by parity of i+j */
            int sign = ((i & 1) ^ (j & 1)) ? -1 : 1;

            seq_t prod = c3[i] * c4[j];
            if (sign < 0)
                prod = -prod;

            total += prod;
        }
    }
    return total;
}
