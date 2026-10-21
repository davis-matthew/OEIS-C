/*
 * A274163  Number of real integers in n-th generation of tree T(4i) defined
 *          in Comments.
 * https://oeis.org/A274163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A274163: Number of real integers in n-th generation of tree T(4i).
   Recurrence:  a(0)=a(1)=a(2)=1;
                for 3 ≤ n < 20: a(n)=a(n-1)+a(n-2);
                for n ≥ 20:     a(n)=a(n-1)+a(n-2)+Fib(n-19),
   where Fib(0)=0, Fib(1)=1.  */

/* seq_t and idx_t are defined by the build. */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1 || n == 2)
        return (seq_t)1;

    /* previous two terms of a(n) */
    seq_t a_prev2 = (seq_t)1;   /* a(n-2) */
    seq_t a_prev1 = (seq_t)1;   /* a(n-1) */
    seq_t cur     = 0;

    /* Fibonacci numbers: fib_prev = Fib(k-1), fib_curr = Fib(k) */
    seq_t fib_prev = (seq_t)0;  /* Fib(0) */
    seq_t fib_curr = (seq_t)1;  /* Fib(1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        if (i < 20)
            cur = a_prev1 + a_prev2;
        else
        {
            cur = a_prev1 + a_prev2 + fib_curr;   /* fib_curr == Fib(i-19) */
            /* advance Fibonacci for the next iteration */
            seq_t fib_next = fib_prev + fib_curr;
            fib_prev = fib_curr;
            fib_curr = fib_next;
        }
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }

    return a_prev1;
}
