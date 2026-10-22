/*
 * A319980  Coordination sequence for (9^3, 3.9^2) net with respect to a
 *          vertex of type 3.9^2.
 * https://oeis.org/A319980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{⌊n/3⌋} (k+1) * c_{n-3k} where c = [1, 3, 4, 6,
 *            6, 3, 3, -2] (coefficient of the generating‑function numerator)
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* coefficients of the rational part of the generating function */
    static const signed char c[8] = {  1,  3,  4,  6,  6,  3,  3, -2 };

    seq_t result = 0;

    /* iterate over all k with 3*k <= n (i.e. k <= n/3) */
    for (idx_t k = 0; k <= n / 3; ++k) {
        idx_t pos = n - 3 * k;          /* n - 3k   */
        if (pos > 7)                /* outside numerator support */
            continue;

        int coeff = c[pos];            /* coefficient for x^(pos) */
        seq_t term = (seq_t)(k + 1) * (seq_t)(coeff >= 0 ? coeff : -coeff);

        if (coeff >= 0)
            result += term;
        else
            result -= term;
    }
    return result;
}
