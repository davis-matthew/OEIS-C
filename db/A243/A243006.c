/*
 * A243006  G.f.: (x - 3*x^2 + 12*x^3 + 6*x^4 - x^5) / ((1 - x) * (1 +
 *          x^2))^2.
 * https://oeis.org/A243006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(4*k) = 27*k * a(4*k+1) = 24*k + 1 * a(4*k+2) = 3*k - 1 *
 *            a(4*k+3) = 6*(k+1) + 1
 * Range:     i8 n <= 19, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n >> 2;   /* k = n/4  */
    switch (n & 3u) {
        case 0u:
            return 27 * (seq_t)k;
        case 1u:
            return 24 * (seq_t)k + 1;
        case 2u:
            return 3 * (seq_t)k - 1;
        case 3u:
            return 6 * (seq_t)(k + 1) + 1;
    }
    return 0; /* unreachable */
}
