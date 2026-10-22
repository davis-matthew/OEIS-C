/*
 * A390525  a(n) = A000217(n)*A000045(n).
 * https://oeis.org/A390525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n+1)/2) * fib(n) where fib(0)=0, fib(1)=1,
 *            fib(n)=fib(n-1)+fib(n-2)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Triangular number T(n) = n*(n+1)/2, computed in seq_t to avoid overflow on idx_t */
    seq_t triangular = (seq_t)n * ((seq_t)n + 1) / 2;

    /* For n == 0 the product is 0 regardless of the Fibonacci value */
    if (n == 0) return 0;

    /* Compute Fibonacci(n) iteratively in O(n) time */
    seq_t f0 = 0;   /* fib(0) */
    seq_t f1 = 1;   /* fib(1) */

    if (n == 1) {
        /* fib(1) = 1, so return T(1)*1 = 1 */
        return triangular;
    }

    for (idx_t i = 2; i <= n; ++i) {
        seq_t tmp = f0 + f1;
        f0 = f1;
        f1 = tmp;
    }

    /* Return the product T(n)*fib(n) */
    return triangular * f1;
}
