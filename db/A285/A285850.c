/*
 * A285850  Number of ways n couples can sit in a row such that exactly one
 *          couple sits next to each other.
 * https://oeis.org/A285850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 2. For n > 1: a(n) = ((4*n^2 - 8*n + 1) *
 *            a(n-1) + (2*n-2)*(2*n-1) * a(n-2)) * 2*n / (2*n-3)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;

    seq_t prev2 = (seq_t)0; /* a(n-2) */
    seq_t prev1 = (seq_t)2; /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t coeff1 = (seq_t)(4*i*i - 8*i + 1);
        seq_t term1 = coeff1 * prev1;

        seq_t coeff2 = (seq_t)((2*i - 2)*(2*i - 1));
        seq_t term2 = coeff2 * prev2;

        seq_t sum = term1 + term2;
        sum *= (seq_t)(2*i);
        sum /= (seq_t)(2*i - 3);

        prev2 = prev1;
        prev1 = sum;
    }

    return prev1;
}
