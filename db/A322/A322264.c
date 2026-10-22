/*
 * A322264  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = denominator of Sum_{d|n} 1/d^k.
 * https://oeis.org/A322264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 69, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = denominator of Σ_{d|r} 1/d^k,
 * where (r,k) are obtained by reading the square array
 * A(r,k) by antidiagonals:
 *   for each diagonal s = r + k (starting with s = 1), 
 *   list entries with r increasing from 1 to s.
 * Then a(1)=1, a(2)=1, a(3)=2, a(4)=1, a(5)=2, …
 *
 * We compute a(n) as follows:
 *   Let s be the smallest integer with s*(s+1)/2 ≥ n.
 *   Let t = n - s*(s-1)/2  (1‑based position in diagonal s).
 *   Then row r = t and exponent k = s - t.
 *   If k == 0 → return 1.
 *   For k > 0:
 *      σ_k(r) = Σ_{d|r} d^k  (sum of k‑th powers of divisors)
 *      Denominator of Σ 1/d^k is r^k / gcd(σ_k(r), r^k).
 *   Return this denominator.
 */

#include <math.h>
#include "../utils/big_pow.h"   /* big_pow(seq_t, seq_t) */
#include "../utils/gcd.h"       /* gcd(seq_t, seq_t) */

/* Helper: triangular number s*(s+1)/2, computed in 128 bit to avoid overflow */
static inline __int128 triangular(idx_t s)
{
    return (__int128)s * (s + 1) / 2;
}

seq_t a(idx_t n)          /* external linkage as required */
{
    /* Find s: smallest integer with triangular(s) >= n */
    long double d = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t s = (idx_t)((d - 1.0L) / 2.0L);          /* floor estimate */
    while ((idx_t)triangular(s) < n) ++s;
    while (s > 0 && (idx_t)triangular(s - 1) >= n) --s;

    idx_t prev_sum = (idx_t)triangular(s - 1);      /* sum of diagonals before s */
    idx_t t = n - prev_sum;                         /* 1‑based position in diagonal s */
    idx_t r = t;                                    /* row index (>=1) */
    idx_t k = s - t;                                /* exponent (>=0) */

    if (k == 0)
        return (seq_t)1;                            /* denominator of integer τ(r) is 1 */

    /* Compute σ_k(r) via prime factorisation of r */
    seq_t sigma = (seq_t)1;
    idx_t tmp = r;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            idx_t e = 0;
            while (tmp % p == 0)
            {
                ++e;
                tmp /= p;
            }
            /* p_pow = p^k */
            seq_t p_pow   = big_pow((seq_t)p, (seq_t)k);
            seq_t term    = (seq_t)1;          /* term = p^(j*k) */
            seq_t sum_term= (seq_t)1;          /* sum j=0..e of p^(j*k) */
            for (idx_t j = 1; j <= e; ++j)
            {
                term *= p_pow;
                sum_term += term;
            }
            sigma *= sum_term;
        }
    }
    if (tmp > 1)                  /* remaining prime factor */
    {
        seq_t p_pow   = big_pow((seq_t)tmp, (seq_t)k);
        seq_t sum_term= (seq_t)1 + p_pow;          /* j=0..1 */
        sigma *= sum_term;
    }

    seq_t r_pow = big_pow((seq_t)r, (seq_t)k);
    seq_t g     = gcd(sigma, r_pow);
    seq_t ans   = r_pow / g;
    return ans;
}
