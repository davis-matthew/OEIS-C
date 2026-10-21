/*
 * A255978  a(n) = a(n-1) + a(n-2) + (1 + (-1)^(a(n-1) + a(n-2))) with a(0)=0,
 *          a(1)=1.
 * https://oeis.org/A255978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*Fibonacci(n) - (1 if n != 0 (mod 3)); initial values:
 *            a(0) = 0, a(1) = 1 (Fibonacci: Fib(0)=0, Fib(1)=1,
 *            Fib(n)=Fib(n-1)+Fib(n-2))
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: n = 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Compute Fibonacci(n) iteratively.
       a0 = Fib(k-1), a1 = Fib(k) after loop iteration k. */
    seq_t a0 = (seq_t)0;      /* Fib(0) */
    seq_t a1 = (seq_t)1;      /* Fib(1) */

    if (n == 1) {
        /* Fib(1) is already in a1 */
    } else {
        for (idx_t k = 2; k <= n; ++k) {
            seq_t tmp = a1;   /* save Fib(k-1) */
            a1 = a0 + a1;     /* Fib(k) = Fib(k-2) + Fib(k-1) */
            a0 = tmp;         /* Fib(k-1) for next iteration */
        }
    }

    /* a1 now holds Fib(n) */
    seq_t fib = a1;

    /* a(n) = 2*Fib(n) - (1 if n % 3 != 0) */
    seq_t result = fib + fib;          /* 2 * Fib(n) */
    if (n % (idx_t)3 != (idx_t)0) {
        result = result - (seq_t)1;
    }

    return result;
}
