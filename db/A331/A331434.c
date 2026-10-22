/*
 * A331434  Column 2 of triangle in A331431.
 * https://oeis.org/A331434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 30*(-1)^n*binomial(n+5,5)
 * Range:     i8 n <= 0, i16 n <= 7, i32 n <= 94, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute C(n+5,5) using a multiplicative formula with 128‑bit
       intermediates to avoid overflow on 64‑bit builds. */
    seq_t binom = 1;
    idx_t start = n + 1;          /* n+1 .. n+5 */
    for (idx_t i = 1; i <= 5; ++i) {
        __int128 tmp = (__int128)binom * (start + i - 1);
        tmp /= i;                 /* division safe, i is 1..5 */
        binom = (seq_t)tmp;
    }

    /* Multiply by 30, then apply sign */
    __int128 tmp = (__int128)binom * 30;
    seq_t res = (seq_t)tmp;
    if (n & 1)              /* odd n => negative */
        res = -res;
    return res;
}
