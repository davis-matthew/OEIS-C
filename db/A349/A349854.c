/*
 * A349854  Expansion of Sum_{k>=0} k^3 * x^k/(1 + k * x).
 * https://oeis.org/A349854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{k=0..n} (-1)^{n-k} * k^{n-k+3}
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 19, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: fast exponentiation for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;               /* seq_t from the build is numeric */
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t exp = n - k + 3;          /* exponent is at least 3 */
        seq_t term = pow_seq((seq_t)k, exp);
        /* (-1)^{n-k} : add if even, subtract if odd */
        if (((n - k) & 1ULL) == 0)
            sum += term;
        else
            sum -= term;
    }
    return sum;
}
