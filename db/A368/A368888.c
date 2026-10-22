/*
 * A368888  a(n) = Sum_{k=0..floor(n/2)} n^(2*k) * binomial(n-k,k).
 * https://oeis.org/A368888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} n^(2*k) * binomial(n-k,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)1;            /* a(0)=1, a(1)=1 */
    idx_t max_k = n / 2;                  /* floor(n/2) */
    seq_t term = (seq_t)1;                /* term for k=0 */
    seq_t sum  = (seq_t)1;                /* sum starts with term_0 */

    for (idx_t k = 1; k <= max_k; ++k)
    {
        /* Compute next term:
           term_k = term_{k-1} * n^2 * (n-2*(k-1)) * (n-2*(k-1)-1)
                    / (k * (n - k + 1))
        */
        seq_t n_sq   = (seq_t)n * (seq_t)n;                /* n^2 */
        seq_t num1   = (seq_t)n - (seq_t)2 * (k - 1);      /* n-2*(k-1) */
        seq_t num2   = num1 - (seq_t)1;                   /* n-2*(k-1)-1 */
        seq_t denom  = (seq_t)k * ((seq_t)n - (seq_t)k + 1); /* k*(n-k+1) */

        seq_t tmp = term;
        tmp *= n_sq;
        tmp *= num1;
        tmp *= num2;
        term = tmp / denom;             /* exact division */

        sum += term;
    }

    return sum;
}
