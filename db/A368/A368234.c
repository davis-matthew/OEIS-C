/*
 * A368234  Number of nondeterministic Dyck excursions of length 2*n.
 * https://oeis.org/A368234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Derived from the generating function * G(x) =
 *            (1-8*x-(1-12*x)*sqrt(1-8*x))/(8*x*(1-9*x)) * The sequence
 *            satisfies a(0)=1, a(1)=4 * and for n≥1 * a(n) = (12*u(n) -
 *            u(n+1) + 72*a(n-1)) / 8 * where u(0)=1 and u(n+1) = u(n) *
 *            4*(2*n-1)/(n+1) * (u(n) are the coefficients of sqrt(1-8*x))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* use signed __int128 for safe intermediate arithmetic */
    __int128 u_prev = 1;   /* u0 */
    /* compute u1 */
    u_prev = u_prev * (4 * (2 * (int64_t)1 - 3)) / 1; /* = -4 */

    __int128 a_prev = 1;   /* a0 */

    for (idx_t k = 1; k <= n; ++k) {
        __int128 u_k = u_prev;                     /* u(k) */
        /* compute u(k+1)   u_next = u_k * 4*(2*k-1)/(k+1) */
        __int128 num   = 4 * (2 * (int64_t)k - 1);
        __int128 denom = (int64_t)k + 1;
        __int128 u_next = u_k * num / denom;

        /* a(k) = (12*u_k - u_next + 72*a_prev) / 8 */
        __int128 a_k = (12 * u_k - u_next + 72 * a_prev) / 8;

        if (k == n) return (seq_t)a_k;

        a_prev = a_k;
        u_prev = u_next;
    }
    return (seq_t)0;  /* unreachable */
}
