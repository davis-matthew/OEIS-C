/*
 * A263823  a(n) = n! * Sum_{k=0..n} Fibonacci(k-1)/k!, where Fibonacci(-1) =
 *          1, Fibonacci(n) = A000045(n) for n>=0.
 * https://oeis.org/A263823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * a(n-1) + Fibonacci(n-1), a(0) = 1 (Obtained from the
 *            definition a(n) = n! * Sum_{k=0..n} F(k-1)/k!)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute the n-th term of A263823 */
seq_t a(idx_t n)
{
    if (n == 0u) {                /* a(0) = 1 */
        return (seq_t)1;
    }

    /* previous term a(k-1) and Fibonacci numbers F(k-1), F(k)  */
    seq_t a_prev  = (seq_t)1;      /* a(0)          */
    seq_t fib_nm1 = (seq_t)0;      /* F(0)          */
    seq_t fib_n   = (seq_t)1;      /* F(1)          */

    for (idx_t k = 1u; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;                /* cast index to seq_t */
        seq_t a_k   = k_seq * a_prev + fib_nm1;/* a(k) = k*a(k-1)+F(k-1)*/

        a_prev = a_k;                         /* shift for next iter */

        /* advance Fibonacci: next two values */
        seq_t fib_next = fib_nm1 + fib_n;
        fib_nm1 = fib_n;
        fib_n   = fib_next;
    }

    return a_prev;                          /* a(n) */
}
