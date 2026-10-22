/*
 * A333292  Triangle read by rows: T(m,n) = Sum_{ 1 <= i <= m, 1 <= j <= n,
 *          gcd(i,j)=1 } i*j, for 1 <= n <= m.
 * https://oeis.org/A333292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,k) where - m = smallest integer with m*(m+1)/2 >= n -
 *            k = n - (m-1)*m/2 - T(m,n) = Sum_{1<=i<=m,1<=j<=n, gcd(i,j)=1}
 *            i*j Using Möbius inversion: T(m,n) = Σ_{d=1}^{m} μ(d) * d² *
 *            S(n/d) * S(m/d) with S(x) = x*(x+1)/2 and μ the Möbius function
 * Range:     u8 n <= 25, u16 n <= 324, u32 n <= 9458, u64 n <= 9458,
 *            u128 n <= 9458, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* compute Möbius μ(n) via trial division, n>=1 */
static inline int8_t mobius(idx_t n)
{
    if (n == 1) return 1;
    int8_t parity = 0;          /* number of distinct prime factors mod 2 */
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            int exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                exp++;
                if (exp > 1) return 0;      /* square factor => μ=0 */
            }
            parity ^= 1;     /* toggle parity for each distinct prime */
        }
    }
    if (temp > 1)            /* remaining prime factor */
        parity ^= 1;
    return parity ? -1 : 1;
}

/* find the row m for a given index n (1‑based) */
static inline idx_t row_from_index(idx_t n)
{
    idx_t low = 1, high = 2;
    /* exponential search for an upper bound */
    while ((__int128)high * (high + 1) / 2 < n)
        high <<= 1;
    /* binary search for smallest m with m*(m+1)/2 >= n */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* compute the nth term of OEIS A333292 */
seq_t a(idx_t n)
{
    /* find row m and column k (1‑based) */
    idx_t m = row_from_index(n);
    idx_t prev = ( (__int128)(m - 1) * m ) / 2;
    idx_t k = n - prev;

    /* compute T(m,k) using the Möbius formula */
    __int128 acc = 0;
    for (idx_t d = 1; d <= m; ++d)
    {
        int8_t mu = mobius(d);
        if (mu == 0) continue;

        idx_t nd = k / d;          /* floor(k/d) */
        idx_t md = m / d;          /* floor(m/d) */

        __int128 Snd = (__int128)nd * (nd + 1) / 2;   /* S(k/d) */
        __int128 Sm  = (__int128)md * (md + 1) / 2;   /* S(m/d) */
        __int128 term = (__int128)d * d;              /* d^2 */
        term *= Snd;
        term *= Sm;

        if (mu > 0)
            acc += term;
        else
            acc -= term;
    }

    return (seq_t)acc;
}
