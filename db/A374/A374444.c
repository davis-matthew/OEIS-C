/*
 * A374444  Array read by ascending antidiagonals: T(n, k) = PS(n, k) where PS
 *          = 1 if n = 1 or k = 1, otherwise PS = 0 if n = 0 or k = 0,
 *          otherwise PS = n^floor(k/2) mods k, where mods is the modulo
 *          operator with the symmetric representation.
 * https://oeis.org/A374444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A374444: 
 * a(n) is the n-th term of the table T(n,k)=PS(n,k) read by ascending 
 * antidiagonals. PS(n,k) = 1 if n=1 or k=1; 0 if n=0 or k=0; 
 * otherwise PS(n,k) = n^floor(k/2) mod k (symmetrically centred modulo).
 */
seq_t a(idx_t n)
{
    /* Find diagonal m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2 */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n) ++m;

    /* offset within this diagonal: t = 0 .. m */
    idx_t t = n - m * (m + 1) / 2;
    /* coordinates along the diagonal: n = m - t, k = t  */
    idx_t row = m - t;   /* first index of T */
    idx_t col = t;       /* second index of T */

    /* Compute PS(n,k) */
    if (row == 1u || col == 1u) return 1;
    if (row == 0u || col == 0u) return 0;

    /* exponent = floor(col/2) */
    idx_t exp = col >> 1;
    seq_t mod = (seq_t)col;

    /* modular exponentiation: (row^exp) mod col */
    seq_t result = 1 % mod;
    seq_t base   = (seq_t)row % mod;
    idx_t e = exp;
    while (e)
    {
        if (e & 1u) result = (result * base) % mod;
        base = (base * base) % mod;
        e >>= 1u;
    }

    /* Symmetric modulo: shift remainder to (-col/2, col/2] */
    seq_t half = (seq_t)(col >> 1u);
    if (result > half) result -= (seq_t)col;

    return result;
}
