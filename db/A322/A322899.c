/*
 * A322899  a(n) = T_{2*n}(n+1) where T_{n}(x) is a Chebyshev polynomial of
 *          the first kind.
 * https://oeis.org/A322899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T_{2n}(n+1) * Chebyshev of the first kind: T_0(x)=1,
 *            T_1(x)=x, T_{k+1}(x)=2*x*T_k(x)-T_{k-1}(x) * Thus
 *            a(n)=2*T_n(n+1)^2-1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute x = n + 1 (the Chebyshev argument) */
    seq_t x = (seq_t)n + (seq_t)1;

    /* a(0) = 1 (since T_0(x)=1 and 2*1^2-1=1) */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Compute T_n(x) iteratively */
    seq_t t_prev = (seq_t)1;   /* T_0(x) */
    seq_t t_cur  = x;          /* T_1(x) */

    for (idx_t k = (idx_t)1; k < n; ++k) {
        /* T_{k+1} = 2*x*T_k - T_{k-1} */
        seq_t t_next = (seq_t)2 * x * t_cur - t_prev;
        t_prev = t_cur;
        t_cur  = t_next;
    }

    /* a(n) = 2 * T_n(x)^2 - 1 */
    seq_t result = (seq_t)2 * t_cur * t_cur - (seq_t)1;
    return result;
}
