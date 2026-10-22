/*
 * A349071  a(n) = T(n, 2*n), where T(n, x) is the Chebyshev polynomial of the
 *          first kind.
 * https://oeis.org/A349071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n, 2*n) where T is the Chebyshev polynomial of the
 *            first kind
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive definition:  T(0,x)=1, T(1,x)=x, T(k+1,x)=2*x*T(k,x)-T(k-1,x) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* x = 2*n  (as seq_t) */
    seq_t x = (seq_t)2 * (seq_t)n;

    /* base cases */
    seq_t t_prev = (seq_t)1;                 /* T(0,x) */
    seq_t t_curr = x;                        /* T(1,x) */

    if (n == 1)
        return t_curr;

    /* iterate recurrence up to n */
    for (idx_t k = 2; k <= n; ++k)
    {
        /* T(k,x) = 2*x*T(k-1,x) - T(k-2,x) */
        seq_t t_next = (seq_t)2 * x * t_curr - t_prev;
        t_prev = t_curr;
        t_curr = t_next;
    }

    return t_curr;
}
