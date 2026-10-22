/*
 * A397105  a(n) = 3^(n-1)*(Fibonacci(n+2) - 1).
 * https://oeis.org/A397105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-1) * (Fib(n+2) - 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS sequence A397105.  */
seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Compute Fibonacci(n+2) iteratively.  */
    seq_t f0 = (seq_t)0;
    seq_t f1 = (seq_t)1;
    for (idx_t i = (idx_t)0; i < n + (idx_t)2; ++i) {
        seq_t tmp = f0 + f1;
        f0 = f1;
        f1 = tmp;
    }
    seq_t fib_n2 = f0;   /* Fib(n+2) */

    /* Compute 3^(n-1) iteratively.  */
    seq_t pow3 = (seq_t)1;
    for (idx_t i = (idx_t)0; i < n - (idx_t)1; ++i)
        pow3 *= (seq_t)3;

    /* Return the final result: 3^(n-1) * (Fib(n+2) - 1).  */
    return pow3 * (fib_n2 - (seq_t)1);
}
