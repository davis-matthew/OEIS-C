/*
 * A320947  a(n) is the number of dominoes, among all domino tilings of the 2
 *          X n rectangle, sharing a length-2 side with the boundary of the
 *          rectangle.
 * https://oeis.org/A320947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*Fibonacci(n-1) (n > 3) Initial
 *            values: a(1)=1, a(2)=4, a(3)=8
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 40, u64 n <= 84, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;

    /* a(n-2) and a(n-1) for the current iteration */
    seq_t a_prev2 = (seq_t)4;      /* a(2)   */
    seq_t a_prev1 = (seq_t)8;      /* a(3)   */

    /* Fibonacci values needed for the recurrence:
       fib_i_minus_2 = Fibonacci(n-2)
       fib_i_minus_1 = Fibonacci(n-1)
       For the loop start (n=4) we need Fib(2) and Fib(3) */
    seq_t fib_i_minus_2 = (seq_t)1;   /* Fibonacci(2) */
    seq_t fib_i_minus_1 = (seq_t)2;   /* Fibonacci(3) */

    /* Compute a(i) for i = 4 .. n */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t a_i = a_prev2 + a_prev1 + (seq_t)2 * fib_i_minus_1;

        /* Shift previous a values for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_i;

        /* Update Fibonacci values for next iteration */
        seq_t fib_i = fib_i_minus_2 + fib_i_minus_1;
        fib_i_minus_2 = fib_i_minus_1;
        fib_i_minus_1 = fib_i;
    }

    return a_prev1;
}
