/*
 * A374653  a(n) = Sum_{k=1..n+1} (k + 1)(n - k + 1)!.
 * https://oeis.org/A374653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n+1} (k + 1)(n - k + 1)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t fact = 1;          /* fact = j! starting with 0! = 1 */

    for (idx_t j = 0; j <= n; ++j) {
        if (j > 0)
            fact = fact * (seq_t)j;          /* update j! */

        seq_t coeff = (seq_t)(n - j + 2);     /* (k+1) with k = n-j+1 */
        sum = sum + coeff * fact;
    }

    return sum;
}
