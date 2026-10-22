/*
 * A366917  a(n) = Sum_{k=1..n} (-1)^k*k^3*floor(n/k).
 * https://oeis.org/A366917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = Sum_{k=1..n} (-1)^k * k^3 * floor(n/k)
 * Range:     i8 n <= 6, i16 n <= 38, i32 n <= 1534, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;                     /* k as seq_t */
        seq_t cube = kk * kk * kk;                /* k^3 */
        seq_t mult = cube * (seq_t)(n / k);       /* k^3 * floor(n/k) */
        if (k & 1) result -= mult;                /* (-1)^k */
        else        result += mult;
    }
    return result;
}
