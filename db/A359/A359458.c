/*
 * A359458  a(n) = A001911(n)*A003266(n+2).
 * https://oeis.org/A359458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Sum_{k=2}^{n+1} Fibonacci(k)) * (Product_{k=2}^{n+2}
 *            Fibonacci(k)), with Fibonacci(k) = A000045(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fibonacci(1) = 1, Fibonacci(2) = 1. */
    seq_t fprev = 1;   /* F(k-1) */
    seq_t fcur  = 1;   /* F(k)   */
    seq_t product = 1;
    seq_t sum = 0;

    /* Iterate k from 2 up to n+2 inclusive.
       At the start of each loop iteration, fcur holds Fibonacci(k). */
    for (idx_t k = 2; k <= n + 2; ++k) {
        /* Include Fibonacci(k) in the sum if it lies in the range 2..n+1. */
        if (k <= n + 1) {
            sum += fcur;
        }
        /* Accumulate the product over 2..n+2. */
        product *= fcur;

        /* Advance to the next Fibonacci number for the next iteration. */
        seq_t next = fprev + fcur;   /* Fibonacci(k+1) */
        fprev = fcur;
        fcur = next;
    }

    return product * sum;
}
