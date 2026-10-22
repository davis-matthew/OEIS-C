/*
 * A344559  a(n) = (1/6) * 2^(-n) * n! * [x^n] Exp(2*x, 1)*(Exp(2*x, 3) - 1),
 *          where Exp(x, m) = Sum_{k>=0} (x^k / k!)^m.
 * https://oeis.org/A344559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/6) * ∑_{k=1}^{⌊n/3⌋} n! / ( (k!)^3 (n-3k)! )
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    idx_t maxk = n / 3;

    /* term for k = 1 : n! / (1!)^3 (n-3)! = n (n-1) (n-2) */
    seq_t term = (seq_t)n * (n - 1) * (n - 2);
    seq_t sum  = term;

    for (idx_t k = 1; k < maxk; ++k)
    {
        seq_t num = (seq_t)(n - 3 * k) * (n - 3 * k - 1) * (n - 3 * k - 2);
        seq_t den = (seq_t)(k + 1) * (k + 1) * (k + 1);
        term = term * num;
        term = term / den;
        sum += term;
    }

    return sum / 6;
}
