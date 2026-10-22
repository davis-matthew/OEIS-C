/*
 * A333094  a(n) is the n-th order Taylor polynomial (centered at 0) of
 *          c(x)^(2*n) evaluated at x = 1, where c(x) = (1 - sqrt(1 -
 *          4*x))/(2*x) is the o.g.f. of the Catalan numbers A000108.
 * https://oeis.org/A333094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (2*n)/(2*n + k)*binomial(2*n + 2*k - 1, k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <limits.h>
/* The implementation uses unsigned __int128 for intermediate arithmetic.
   It works for the uint64_t build (using the 128‑bit type) and also for
   arbitrary‑precision builds where seq_t can be constructed from an
   unsigned integer. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* local typedef for 128‑bit intermediate */
    typedef unsigned __int128 u128;

    idx_t two_n = 2 * n;
    seq_t sum = 0;
    u128 bin = 1;              /* C(2n-1,0) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t denom = (seq_t)(two_n + k);                     /* 2*n + k */
        u128 term128 = bin * (u128)two_n / (u128)denom;       /* term_k */
        sum += (seq_t)term128;                               /* accumulate */

        if (k == n)
            break;                                            /* finished */

        /* update bin to C(2n + 2*(k+1) - 1, k+1) */
        idx_t m = two_n + 2 * k - 1;                          /* current m */
        bin    = bin * (u128)(m + 1);                         /* * (m+1) */
        bin    = bin * (u128)(m + 2);                         /* * (m+2) */
        bin    = bin / (u128)(k + 1);                         /* / (k+1) */
        bin    = bin / (u128)(two_n + k);                     /* / (2*n + k) */
    }

    return sum;
}
