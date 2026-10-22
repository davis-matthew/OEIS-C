/*
 * A338995  Triangle T(n,m):=binomial(n+3*m+2,n-m).
 * https://oeis.org/A338995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r,c are row/column of the triangle * with
 *            T(r,c) = binomial(r + 3*c + 2 , r - c). * Row index r is the
 *            unique integer satisfy r*(r+1)/2 ≤ n < (r+1)*(r+2)/2, * and
 *            column index c = n – r*(r+1)/2. This yields the OEIS sequence *
 *            A338995: 1,3,1,6,7,1,10,28,11,1,15,84,66,15,1,21,2...
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* multiplicative binomial coefficient C(n,k) for any seq_t implementation */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        /* multiply first to avoid rounding errors */
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine row r via integer formula r = floor((sqrt(8*n+1)-1)/2) */
    idx_t r = (idx_t)((sqrt((long double)(8LL * (long double)n + 1)) - 1) / 2);

    /* Correct possible round‑off errors (unlikely but safe) */
    /* Compute offset = r*(r+1)/2 using 128‑bit arithmetic to avoid overflow */
    __uint128_t offset128 = (__uint128_t)r * (r + 1ULL) / 2;
    idx_t offset = (idx_t)offset128;
    while (( (__uint128_t)(r + 1ULL) * (r + 2ULL) / 2) <= n)
    {
        ++r;
        offset128 = (__uint128_t)r * (r + 1ULL) / 2;
        offset = (idx_t)offset128;
    }
    while (( (__uint128_t)r * (r + 1ULL) / 2) > n)
    {
        --r;
        offset128 = (__uint128_t)r * (r + 1ULL) / 2;
        offset = (idx_t)offset128;
    }

    /* Column index c (here called m in description) */
    idx_t m = n - offset;

    /* Compute T(r,m) = binomial(r + 3*m + 2 , r - m) */
    return binom(r + 3 * m + 2, r - m);
}
