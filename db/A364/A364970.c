/*
 * A364970  a(n) = Sum_{k=1..n} binomial(floor(n/k)+2,3).
 * https://oeis.org/A364970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} binomial(k+1,2) * floor(n/k)
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses only integer arithmetic, no floating point. */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* binomial(k+1,2) = k*(k+1)/2 */
        seq_t bin = (seq_t)k * ((seq_t)k + 1) / 2;
        sum += bin * (seq_t)(n / k);
    }
    return sum;
}
