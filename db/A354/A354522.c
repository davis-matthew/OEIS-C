/*
 * A354522  Square array A(n, k), n, k >= 0, read by antidiagonals; A(n, k) =
 *          g(f(n) + f(k)) where f denotes A001057 and g denotes its inverse.
 * https://oeis.org/A354522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A354522:
   Compute the square array A(n,k) by antidiagonal order:
   A(n,k) = g( f(n) + f(k) ),
   where f is the bijection A001057:
     f(m) =  (m odd  ? (m+1)/2  : -(m/2) )
   and g is its inverse:
     g(x) = ( x <= 0 ? -2*x : 2*x-1 )
   The index n (0‑based) is converted to coordinates (i,j)
   with i + j = d where d is the largest integer satisfying d*(d+1)/2 <= n,
   and i = n - d*(d+1)/2, j = d - i.  The function returns A(i,j). */
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Find d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t low = 0;
    /* Upper bound: d <= floor((sqrt(8n+1)-1)/2) <= sqrt(2n) */
    double nd = (double)n;
    idx_t high = (idx_t)(sqrt(2.0*nd) + 3.0);  /* safety margin */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;      /* (low+high+1)/2 */
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t d = low;
    __uint128_t pre128 = (__uint128_t)d * (d + 1) / 2;
    idx_t pre   = (idx_t)pre128;           /* pre = d*(d+1)/2 */
    idx_t i = n - pre;
    idx_t j = d - i;

    /* Compute f(i) and f(j) using signed 64‑bit arithmetic */
    int64_t fi = (i & 1) ? (int64_t)((i + 1) >> 1) : -(int64_t)(i >> 1);
    int64_t fj = (j & 1) ? (int64_t)((j + 1) >> 1) : -(int64_t)(j >> 1);
    int64_t sum = fi + fj;

    /* g(sum) = -2*sum if sum <= 0, else 2*sum-1 */
    seq_t res;
    if (sum <= 0)
        res = (seq_t)(-2 * sum);
    else
        res = (seq_t)(2 * sum - 1);

    return res;
}
