/*
 * A242496  a(n)=sum_{j=0..n} sum_{i=0..j} F(i)*L(j), where F(n)=A000045(n)
 *          and L(n)=A000032(n).
 * https://oeis.org/A242496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{j=0..n} (sum_{i=0..j} F(i)) * L(j) where F(i) is the
 *            Fibonacci sequence (F(0)=0,F(1)=1) and L(j) is the Lucas
 *            sequence (L(0)=2,L(1)=1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fibonacci: f_prev=F(j), f_curr=F(j+1)                */
    /* Lucas:     l_prev=L(j), l_curr=L(j+1)                */
    seq_t f_prev = (seq_t)0, f_curr = (seq_t)1; /* F(0), F(1) */
    seq_t l_prev = (seq_t)2, l_curr = (seq_t)1; /* L(0), L(1) */

    seq_t cumulative_fib = (seq_t)0;       /* Σ_{i=0..j} F(i)   */
    seq_t result = (seq_t)0;                /* running sum of a(n) */

    for (idx_t j = 0; j <= n; ++j)
    {
        seq_t Fj = f_prev;    /* Fibonacci at index j   */
        seq_t Lj = l_prev;    /* Lucas at index j       */

        cumulative_fib += Fj;                    /* Σ_{i=0..j} F(i)        */
        result += cumulative_fib * Lj;            /* add term for current j */

        /* Advance to next indices for next loop iteration */
        seq_t f_next = f_prev + f_curr;           /* F(j+2) */
        seq_t l_next = l_prev + l_curr;           /* L(j+2) */

        f_prev = f_curr;
        f_curr = f_next;

        l_prev = l_curr;
        l_curr = l_next;
    }

    return result;
}
