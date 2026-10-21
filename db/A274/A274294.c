/*
 * A274294  a(n) = 1+(n+1)^2+n!+Sum_{k=1..n-1} binomial(n,k)*n!/(n-k)!.
 * https://oeis.org/A274294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (n + 1)^2 + n! + ∑_{k=1}^{n-1}
 *            binomial(n,k)·n!/(n-k)!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Compute n! = fact
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    // term_i' = n! * binom(n,i)/i! ; term0' = n!
    seq_t term = fact;
    seq_t sum  = term;          // sum over i=0..n-1

    for (idx_t i = 1; i < n; ++i)
    {
        // term_i = term_{i-1} * (n - i + 1) / (i * i)
        seq_t numerator   = term * (seq_t)(n - i + 1);
        seq_t denominator = (seq_t)i * (seq_t)i;
        term = numerator / denominator;
        sum += term;
    }

    seq_t np1 = (seq_t)(n + 1);
    seq_t a_val = 1 + np1 * np1 + sum;
    return a_val;
}
