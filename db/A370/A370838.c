/*
 * A370838  Expansion of (1/x) * Series_Reversion( x/(x+1/(1+x^4)) ).
 * https://oeis.org/A370838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} (-1)^k * binom(n,4*k) *
 *            binom(5*k,k)/(4*k+1)
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 23, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - i + 1);
        res = res / (seq_t)i;           /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term = binom_idx(n, 4 * k);
        term = term * binom_idx(5 * k, k);
        term = term / (seq_t)(4 * k + 1);
        if (k & 1) sum -= term;
        else      sum += term;
    }
    return sum;
}
