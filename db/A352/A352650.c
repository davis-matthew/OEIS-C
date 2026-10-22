/*
 * A352650  Triangle read by rows: T(n,k) = n * T(n-1,k) + (-1)^(n-k) for 0 <=
 *          k <= n with initial values T(n,k) = 0 if n < 0 or k < 0 or k > n.
 * https://oeis.org/A352650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r , j) where r = floor((sqrt(8*n + 1) - 1) / 2), j = n
 *            - r*(r+1)/2, and T(n,k) is defined by T(n,k) + T(n,k+1) = n! /
 *            k! (0 ≤ k < n) with T(n,n) = 1
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static inline idx_t _row_of(idx_t n)
{
    /* find r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    __uint128_t t = (__uint128_t)n * 8 + 1;          /* 8*n + 1  */
    long double s = sqrt((long double)t);           /* sqrt(8n+1)  */
    idx_t r = (idx_t)((s - 1.0L) / 2.0L);           /* floor((sqrt-1)/2) */
    /* correct possible rounding errors */
    while (( (__uint128_t)(r + 1) * (r + 2) ) / 2 <= n) r++;
    while (( (__uint128_t)r * (r + 1) ) / 2 > n) r--;
    return r;
}

static inline seq_t _T_value(idx_t n, idx_t k)
{
    /* Compute T(n,k) using the identity
         T(n,k)+T(n,k+1)=n! / k!    with T(n,n)=1       */
    seq_t P = 1;      /* product of numbers from k+1 … n */
    seq_t t = 1;      /* T(n, current k)  */
    for (idx_t cur = n; cur > k; --cur) {
        P = P * (seq_t)cur;    /* P for (cur-1) = P * cur */
        t = P - t;
    }
    return t;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* T(0,0)=1 */

    idx_t r = _row_of(n);                      /* row index */
    seq_t rfac = (seq_t)r * (seq_t)(r + 1);
    rfac /= 2;                                 /* r*(r+1)/2  */
    idx_t j = (idx_t)(n - rfac);               /* column index */

    return _T_value(r, j);
}
