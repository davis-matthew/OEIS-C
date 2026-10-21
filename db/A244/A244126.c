/*
 * A244126  Triangle read by rows: coefficients T(n,k) of a binomial
 *          decomposition of 2^n-1 as Sum(k=0..n)T(n,k)*binomial(n,k).
 * https://oeis.org/A244126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, T(r,0) = 0, T(r,k) = (1+k)^(k-1) * (1-k)^(r-k) for
 *            k>0 (OEIS A244126)
 * Range:     i8 n <= 18, i16 n <= 33, i32 n <= 64, i64 n <= 167,
 *            i128 n <= 431, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute x^e for non‑negative e using binary exponentiation.
   The exponent is of type idx_t; the base and result use seq_t. */
static inline seq_t pow_int(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp != 0)
    {
        if (exp & 1u)
            result *= base;
        base *= base;
        exp >>= 1u;
    }
    return result;
}
 
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2   */
    idx_t r = 0;
    while (((__int128)(r + 1) * (r + 2)) / 2 <= n)
        ++r;
 
    /* Compute column index k in this row */
    idx_t k = n - r * (r + 1) / 2;
 
    if (k == 0)
        return (seq_t)0;          /* T(r,0)=0 by convention */
 
    /* Bases for the two power terms */
    seq_t base1 = (seq_t)1 + (seq_t)k;      /* 1 + k   */
    seq_t base2 = (seq_t)1 - (seq_t)k;      /* 1 - k   */
 
    /* Exponents for the two power terms */
    idx_t exp1 = k - 1;          /* k-1   (>=0)   */
    idx_t exp2 = r - k;          /* r-k   (>=0)   */
 
    seq_t term1 = pow_int(base1, exp1);
    seq_t term2 = pow_int(base2, exp2);
 
    return term1 * term2;
}
