/*
 * A318702  For any n >= 0 with binary expansion Sum_{k=0..w} b_k * 2^k, let
 *          f(n) = Sum_{k=0..w} b_k * i^k * 2^floor(k/2) (where i denotes the
 *          imaginary unit); a(n) is the real part of f(n).
 * https://oeis.org/A318702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Σ_{m≥0} (-1)^m * 2^m * bit_{2m}(n) (real part of f(n)
 *            where f(n) = Σ b_k * i^k * 2^⌊k/2⌋)
 * Range:     i8 n <= 7994, i16 n <= 7994, i32 n <= 7994, i64 n <= 7994,
 *            i128 n <= 7994, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res   = 0;
    seq_t power = 1;
    int  sign   = 1;
    while (n != 0) {
        if ((n & 1U) != 0)
            res += ((seq_t)sign) * power;
        power <<= 1;
        sign = -sign;
        n >>= 2;
    }
    return res;
}
