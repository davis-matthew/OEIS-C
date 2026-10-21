/*
 * A242091  a(n) = r * (n-1)! where r is the rational number that satisfies
 *          the equation Sum_{k>=n} (-1)^(k + n)/C(k,n) = n*2^(n-1)*log(2) -
 *          r.
 * https://oeis.org/A242091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{j=1}^{n-1} 2^{n-j-1}/j Recurrence: a(1)=0,
 *            a(n+1)=2*(n+1)*a(n)+(n+1)!/n computed iteratively to avoid heavy
 *            caching and for arbitrary‑precision support
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t a_val = 0;   /* a(1) = 0 */
    seq_t fact  = 1;   /* fact holds m! for current m starting at m=1 */

    for (idx_t m = 1; m < n; ++m) {
        /* term = (m-1)! * (m+1) = (m! / m) * (m+1) */
        seq_t term = fact / (seq_t)m;
        term *= (seq_t)(m + 1);

        /* a_{m+1} = 2*(m+1)*a_m + term */
        a_val = (a_val * (seq_t)(2 * (m + 1))) + term;

        /* update factorial to (m+1)! for next iteration */
        fact *= (seq_t)(m + 1);
    }

    return a_val;
}
