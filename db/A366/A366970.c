/*
 * A366970  a(n) = Sum_{k=3..n} binomial(k-1,2) * floor(n/k).
 * https://oeis.org/A366970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=3..n} binomial(k-1, 2) * floor(n/k)
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t sum = 0;

    for (idx_t k = 3; k <= n; ++k)
    {
        /* binomial(k‑1, 2) = (k-1)*(k-2)/2 */
        seq_t bin = ((seq_t)(k - 1) * (seq_t)(k - 2)) / 2;
        seq_t q   = (seq_t)(n / k);      /* floor division */

        sum += bin * q;
    }

    return sum;
}
