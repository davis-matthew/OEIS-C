/*
 * A375500  a(n) = Sum_{k=0..n} A001595(k)^2.
 * https://oeis.org/A375500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(Fibonacci(n+1)-1)*(Fibonacci(n+2)-1) + n + 1
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute Fibonacci numbers F(n+1) and F(n+2) using only +. */
    seq_t a_f = 0, b_f = 1;          /* a_f = F(k-2), b_f = F(k-1) */
    idx_t limit = n + 2;              /* compute up to F(n+2) */
    for (idx_t i = 2; i <= limit; ++i)
    {
        seq_t c = a_f + b_f;
        a_f = b_f;
        b_f = c;
    }
    seq_t f_n1 = a_f;                /* F(n+1) */
    seq_t f_n2 = b_f;                /* F(n+2) */

    /* a(n) = 4*(f_n1-1)*(f_n2-1) + n + 1 */
    seq_t term1 = f_n1 - 1;
    seq_t term2 = f_n2 - 1;
    seq_t prod  = term1 * term2;
    prod *= 4;
    prod += (seq_t)n;
    prod += 1;
    return prod;
}
