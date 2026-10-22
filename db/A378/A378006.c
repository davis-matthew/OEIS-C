/*
 * A378006  Square table read by descending antidiagonals: the k-th column has
 *          Dirichlet g.f. Product_{chi} L(chi,s), where chi runs through all
 *          Dirichlet characters modulo k.
 * https://oeis.org/A378006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 8315, u16 n <= 8315, u32 n <= 8315, u64 n <= 8315,
 *            u128 n <= 8315, big any n
 * Helpers:   binomial, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A378006: Square table read by descending antidiagonals.
   a(n) is the entry T(row, col) with 
      row + col = s where s is the smallest integer with s(s-1)/2 >= n,
   offset 1 (a(1) = 1).  T(n,k) is multiplicative in n.  For each prime p|n
   let e = exponent of p in n.  If p | k  => T(n,k)=0.  
   Let ord = multiplicative order of p modulo k, and φ(k) Euler's totient.
   If e is not divisible by ord => T(n,k)=0.
   Otherwise T(n,k) = Π_p binom( e/ord + φ(k)/ord - 1 , φ(k)/ord - 1 )
   (product over all prime factors p of n).
*/

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/powmod64.h"          /* powmod64(base,exp,mod)      */
#include "../utils/binomial.h"          /* binomial(idx_t,idx_t)       */

/* integer square root of a 128‑bit unsigned integer */
static uint64_t isqrt_u128(unsigned __int128 x)
{
    long double d = sqrtl((long double)x);
    uint64_t r = (uint64_t)d;
    unsigned __int128 rr = r;
    while (rr * rr > x) { r--; rr = r; }
    while ((unsigned __int128)(r + 1) * (r + 1) <= x) { r++; }
    return r;
}

/* Euler totient φ(m) for unsigned 64‑bit integers */
static uint64_t phi_uint64(uint64_t m)
{
    uint64_t res = m;
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            while (m % p == 0) m /= p;
            res = res / p * (p - 1);
        }
    }
    if (m > 1) res = res / m * (m - 1);
    return res;
}

/* multiplicative order of a modulo k,  gcd(a,k)=1 */
static uint64_t multiplicative_order(uint64_t a, uint64_t k)
{
    uint64_t phi_k = phi_uint64(k);
    uint64_t ord = phi_k;
    uint64_t tmp = phi_k;

    for (uint64_t d = 2; d * d <= tmp; ++d)
    {
        if (tmp % d == 0)
        {
            while (tmp % d == 0) tmp /= d;
            while (ord % d == 0)
            {
                uint64_t cand = ord / d;
                if (powmod64(a, cand, k) == 1) ord = cand;
                else break;
            }
        }
    }
    if (tmp > 1)
    {
        uint64_t d = tmp;
        while (ord % d == 0)
        {
            uint64_t cand = ord / d;
            if (powmod64(a, cand, k) == 1) ord = cand;
            else break;
        }
    }
    return ord;
}

/* multiplicative term T(n,k) */
static seq_t T(uint64_t n, uint64_t k)
{
    if (n == 1) return 1;
    seq_t result = 1;
    uint64_t phi_k = phi_uint64(k);
    uint64_t tmp = n;

    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            if (k % p == 0) return 0;
            uint64_t ord = multiplicative_order(p, k);
            if (e % ord != 0) return 0;
            uint64_t t = phi_k / ord;
            uint64_t r = e / ord;
            seq_t val = binomial(r + t - 1, t - 1);
            result *= val;
        }
    }

    if (tmp > 1)
    {
        uint64_t p   = tmp;
        if (k % p == 0) return 0;
        int e = 1;
        uint64_t ord = multiplicative_order(p, k);
        if (e % ord != 0) return 0;
        uint64_t t = phi_k / ord;
        uint64_t r = e / ord;
        seq_t val = binomial(r + t - 1, t - 1);
        result *= val;
    }

    return result;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1: undefined for n==0 */
    uint64_t m = n;                  /* m = npos */

    /* find smallest s with s*(s-1)/2 >= m */
    unsigned __int128 val = (unsigned __int128)1 + (unsigned __int128)8 * m;
    uint64_t sqrt_val = isqrt_u128(val);
    uint64_t s = (1 + sqrt_val) / 2;
    while ((unsigned __int128)s * (s - 1) / 2 < m) ++s;

    /* index inside this antidiagonal (0‑based) */
    unsigned __int128 prev_cnt_128 = (unsigned __int128)(s - 1) * (s - 2) / 2;
    uint64_t prev_cnt = (uint64_t)prev_cnt_128;
    uint64_t offset   = m - prev_cnt - 1;   /* 0‑based */
    uint64_t row = offset + 1;              /* n (row)    */
    uint64_t col = s - row;                 /* k (column) */

    return T(row, col);
}
