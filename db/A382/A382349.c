/*
 * A382349  a(n) = [x^n] Product_{k=0..n} (1 + (3*n+k)*x).
 * https://oeis.org/A382349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0..n} (∏_{k=0..n} (3*n + k) / (3*n + j)) (with
 *            a(0)=1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* product P = ∏_{k=0..n} (3*n + k), i.e. (4*n)!/(3*n)!   */
    idx_t start = 3 * n;
    idx_t end   = 4 * n;
    seq_t P = (seq_t)1;
    for (idx_t k = start; k <= end; ++k)
        P *= (seq_t)k;

    /* sum over the n+1 reciprocal terms */
    seq_t sum = (seq_t)0;
    for (idx_t j = 0; j <= n; ++j)
        sum += P / (seq_t)(start + j);

    return sum;
}
