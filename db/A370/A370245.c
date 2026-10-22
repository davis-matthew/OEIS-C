/*
 * A370245  Coefficient of x^n in the expansion of ( 1/(1-x)^2 * (1+x^2)^3
 *          )^n.
 * https://oeis.org/A370245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(3*n, k) * * binomial(3*n-2*k-1,
 *            n-2*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute binomial coefficient C(a,b) using a multiplicative
 * algorithm that only uses integer arithmetic.  The routine
 * works for any seq_t type that supports +,-,*,/,%. */
static seq_t binom_simple(idx_t a, idx_t b)
{
    if (b > a) return 0;
    if (b > a - b) b = a - b;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i)
    {
        res = res * (seq_t)(a - b + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;              /* a(0) = 1 */

    seq_t sum = 0;
    idx_t maxk = n / 2;
    seq_t three_n = (seq_t)(3 * n);

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term1 = binom_simple((idx_t)three_n, k);
        seq_t term2 = binom_simple((idx_t)(three_n - (seq_t)(2 * k) - 1),
                                   (idx_t)(n - 2 * k));
        sum += term1 * term2;
    }
    return sum;
}
