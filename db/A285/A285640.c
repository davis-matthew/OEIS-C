/*
 * A285640  a(n) = negative of exponent of s(2) in factorization of s(n) where
 *          s(1)^2 * s(n+2) * s(n-2) = s(2)^2 * s(n+1) * s(n-1) - s(3) * s(1)
 *          * s(n)^2, s(n) = -s(-n) for all n in Z, with a(0) = -2, and s(1),
 *          s(2), s(3), s(4) are variables.
 * https://oeis.org/A285640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is given by the generating function G(x) = (-2 + 4x - 3x² +
 *            2x³ - x⁴ + x⁶ - 2x⁷ + 3x⁸ - 3x⁹ + 3x¹⁰ - 2x¹¹ + x¹² - x¹⁴ + 2x¹⁵
 *            - x¹⁶) / ((1 - x)² (1 - x¹⁶)) The coefficient of xⁿ equals a(n).
 *            This expands to a(n) = Σ_{d∈D} c_d * Σ_{j=0}^{⌊(n-d)/16⌋}
 *            (n-d-16j + 1), where (d, c_d) are the degree...
 * Range:     i8 n <= 64, i16 n <= 1024, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Degree‑coefficient pairs of the numerator polynomial N(x) */
    static const idx_t d[17]   = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16};
    static const int    c[17]   = {-2,  4, -3,  2, -1, 0,  1, -2,  3, -3,  3, -2, 1, 0, -1, 2, -1};

    seq_t result = 0;
    for (int i = 0; i < 17; ++i)
    {
        if (n < d[i])          /* terms with d > n have zero contribution */
            continue;

        idx_t diff = n - d[i];          /* n - d */
        idx_t m    = diff / 16;         /* floor((n-d)/16) */
        idx_t A    = diff + 1;          /* n - d + 1 */

        /* Compute (m+1)(A-8m) first to keep intermediate products small. */
        idx_t term1 = m + 1;
        idx_t term2 = A - 8 * m;

        /* Multiply by the coefficient c[i] and add to the result. */
        seq_t term = (seq_t)c[i] * (seq_t)term1 * (seq_t)term2;
        result += term;
    }
    return result;
}
