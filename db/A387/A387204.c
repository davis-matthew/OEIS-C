/*
 * A387204  a(n) = n*(n + 1)*(n^2 + 17*n + 54)/24.
 * https://oeis.org/A387204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 1)*(n^2 + 17*n + 54)/24 * Offset: 0 * This
 *            function works for any n ≥ 0 and returns the exact value *
 *            whenever it fits in the seq_t type. It uses 128‑bit arithmetic *
 *            for intermediate results to avoid overflow on small seq_t sizes
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 562, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit integers for intermediate calculations */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 t1 = N + 1U;                       /* n + 1 */
    unsigned __int128 t2 = N * N + 17U * N + 54U;        /* n^2 + 17*n + 54 */
    unsigned __int128 num = N * t1;                      /* n*(n + 1) */
    num *= t2;                                            /* multiplied by the quadratic term */
    num /= 24U;                                           /* final division by 24 */

    return (seq_t)num;                                     /* cast back to seq_t */
}
