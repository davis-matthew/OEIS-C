/*
 * A379588  Antidiagonal sums of the array A379587.
 * https://oeis.org/A379588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n+2} (k^(n-k+2) - 1)^2/(k - 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_local(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 2; k <= n + 2; ++k)
    {
        idx_t exp_index = n - k + 2;          /* n - k + 2 >= 0 */
        seq_t base = (seq_t)k;
        seq_t p = pow_seq_local(base, exp_index);    /* k^(n-k+2) */
        seq_t p_minus_1 = p - 1;
        seq_t numer = p_minus_1 * p_minus_1;        /* (k^(n-k+2)-1)^2 */
        seq_t denom = base - 1;                     /* k - 1 */
        seq_t term = numer / denom;                 /* integer division */
        sum += term;
    }
    return sum;
}
