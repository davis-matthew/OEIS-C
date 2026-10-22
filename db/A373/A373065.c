/*
 * A373065  Expansion of e.g.f. (1/2)*(x^2*exp(x))*(cosh(x))^2.
 * https://oeis.org/A373065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,2) * (3^(n-2) + (-1)^n + 2) / 4
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Set to 0 for n = 0,1) */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* binomial(n,2) = n*(n-1)/2  */
    seq_t bin = ((seq_t)n * ((seq_t)n - 1)) / 2;

    /* compute 3^(n-2) by repeated multiplication */
    seq_t pow3 = (seq_t)1;
    for (idx_t i = 0; i < n - 2; ++i)
        pow3 *= (seq_t)3;

    /* (-1)^n + 2 : 3 if n even, 1 if n odd */
    seq_t add = ((n & 1) == 0) ? (seq_t)3 : (seq_t)1;

    /* numerator of the fraction */
    seq_t numer = pow3 + add;

    /* numer is always divisible by 4, so divide first */
    seq_t numer4 = numer >> 2;   /* same as numer / 4 */

    /* final result */
    return bin * numer4;
}
