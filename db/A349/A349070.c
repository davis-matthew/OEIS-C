/*
 * A349070  a(n) = T(3*n, n), where T(n, x) is the Chebyshev polynomial of the
 *          first kind.
 * https://oeis.org/A349070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(3*n, n) (Chebyshev polynomial of first kind)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case: a(0) = T(0,0) = 1 */
    if (n == 0) return (seq_t)1;

    /* x = n (the argument of the Chebyshev polynomial) */
    seq_t x = (seq_t)n;

    /* T0 = T(0,x) = 1, T1 = T(1,x) = x */
    seq_t T0 = (seq_t)1;
    seq_t T1 = x;

    /* Precompute 2*x, which is used at every step */
    seq_t two_x = (seq_t)2 * x;

    /* Iterate recurrence T(k+1) = 2*x*T(k) - T(k-1) up to k = 3*n */
    for (idx_t k = 2; k <= 3 * n; ++k)
    {
        seq_t next = two_x * T1 - T0; /* next term */
        T0 = T1;                     /* shift previous term */
        T1 = next;                   /* update current term */
    }

    return T1; /* T(3*n, n) */
}
