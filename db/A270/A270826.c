/*
 * A270826  Maximum number of iterations needed in the Euclid algorithm for
 *          gcd(x,y) in [1..n].
 * https://oeis.org/A270826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A270826: a(n) = ceil(log(n*sqrt(5)) / log((sqrt(5)+1)/2)) - 2
 *
 * Computation method
 * ------------------
 * For Fibonacci numbers F_m with F_0 = 0, F_1 = 1 we have the identity
 *
 *      ceil(phi^m / sqrt(5)) = F_m + (m even ? 1 : 0)
 *
 * (phi = (1+sqrt(5))/2).  Therefore a(n)+2 is the smallest m such that
 * n < F_m + (m even ? 1 : 0).  This gives an exact integer algorithm that
 * uses only integer arithmetic and the Fibonacci recurrence.
 *
 * The routine loops over m, maintaining consecutive Fibonacci values, until
 * the threshold is exceeded.  The number of iterations is O(log n), which
 * is far below the required time limit for all supported input sizes.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be at least 1 by the problem statement. */
    idx_t        m      = 0;
    seq_t        fib_prev = 0;   /* F_0 */
    seq_t        fib_curr = 1;   /* F_1 */
    while (1)
    {
        seq_t f_m = (m == 0) ? fib_prev : fib_curr;
        seq_t threshold = f_m + ((m & 1U) == 0 ? 1 : 0);

        if ((seq_t)n < threshold) {
            /* a(n) = m - 2, result fits in seq_t for all n ≥ 1. */
            return (seq_t)(m - 2);
        }

        /* Advance to the next Fibonacci index. */
        m++;
        if (m > 1) {
            seq_t next = fib_prev + fib_curr; /* F_{m-1} + F_{m-2} = F_m */
            fib_prev = fib_curr;
            fib_curr = next;
        }
    }
}
