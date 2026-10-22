/*
 * A329288  Table T(n,k) read by antidiagonals: T(n,k) = f(T(n,k-1)) starting
 *          with T(n,1)=n, where f(x) = x - 1 + x/gpf(x), that is, f(x) =
 *          A269304(x)-2.
 * https://oeis.org/A329288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th value read by antidiagonal of the table T
 *            defined by T(n,1)=n, T(n,k)=f(T(n,k−1)) with f(x)=x−1+x/gpf(x)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/gpf.h"

static seq_t f_seq(seq_t x)
{
    if (x <= (seq_t)1)
        return (seq_t)1;
    uint64_t xu = (uint64_t)x;          /* value fits in 64 bits for the tested range */
    uint64_t g  = gpf(xu);              /* largest prime factor of xu */
    seq_t  gs  = (seq_t)g;              /* cast to seq_t for division */
    return x - (seq_t)1 + x / gs;       /* f(x) = x−1 + x/gpf(x) */
}

seq_t a(idx_t n)
{
    /* Find the diagonal d such that (d−1)d/2 < n ≤ d(d+1)/2 */
    idx_t d = (idx_t)((sqrt((long double)8.0L * n + 1.0L) - 1.0L) / 2.0L);
    while (d * (d + 1) / 2 < n) d++;
    while (d > 0 && (d - 1) * d / 2 >= n) d--;

    idx_t prev   = (d - 1) * d / 2;                /* elements before diagonal d */
    idx_t offset = n - prev - 1;                  /* zero‑based offset on diagonal d */
    idx_t i = offset + 1;                         /* row index (starting value)     */
    idx_t j = d - offset;                         /* column index (number of f’s)   */

    seq_t val = (seq_t)i;                         /* start with T(i,1)=i            */
    for (idx_t step = 1; step < j; ++step)        /* apply f to reach T(i,j)       */
        val = f_seq(val);

    return val;
}
