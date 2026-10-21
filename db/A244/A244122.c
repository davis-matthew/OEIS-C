/*
 * A244122  Triangle read by rows: coefficients T(n,k) of a binomial
 *          decomposition of n^n as Sum(k=0..n)T(n,k)*binomial(n,k).
 * https://oeis.org/A244122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 19, i32 n <= 43, i64 n <= 117,
 *            i128 n <= 320, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A244122 – Triangle read by rows: coefficients T(n,k) of a binomial
 *  decomposition of n^n.  For k>0,
 *        T(n,k) = n*(n+k)^(k-1)*(-k)^(n-k),
 *  while T(n,0) = 0^n (1 when n=0, otherwise 0).
 *
 *  a(n) enumerates the triangle row‑by‑row:
 *        a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …
 *  Given the index n, the row r and column k are:
 *        r = floor((sqrt(1+8*n)-1)/2),
 *        k = n - r*(r+1)/2
 *  Then a(n)=T(r,k).
 */

#include <stdint.h>
#include <math.h>

/* integer power for seq_t (works for signed and arbitrary‐precision types) */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* floor(sqrt(x)) for 0 <= x <= 2^127-1 using double approximation and
   correction with 128‑bit arithmetic.  This is safe for n up to the limits
   of 64‑bit indices. */
static uint64_t isqrt_u128(__int128 x)
{
    double d = sqrt((double)x);
    uint64_t s = (uint64_t)d;
    /* Adjust downward if necessary */
    while ((__int128)s * s > x)
        s--;
    /* Adjust upward if necessary */
    while ((__int128)(s + 1U) * (s + 1U) <= x)
        s++;
    return s;
}

seq_t a(idx_t n)
{
    /* Compute row r and column k corresponding to the linear index n */
    __int128 val = (__int128)8 * n + 1;
    uint64_t sq   = isqrt_u128(val);
    uint64_t r    = (sq - 1U) >> 1U;          /* floor((sqrt(1+8n)-1)/2) */
    uint64_t prev = r * (r + 1U) / 2U;
    uint64_t k    = n - prev;

    /* Handle the special case k == 0: T(r,0) = 0^r, 0^0 = 1 */
    if (k == 0U)
        return (r == 0U) ? (seq_t)1 : (seq_t)0;

    /* Compute T(r,k) = r*(r+k)^(k-1)*(-k)^(r-k) */
    seq_t base1 = (seq_t)(r + k);
    seq_t pow1  = pow_seq_t(base1, (idx_t)(k - 1U));

    seq_t base2 = (seq_t)(-(int64_t)k);
    seq_t pow2  = pow_seq_t(base2, (idx_t)(r - k));

    seq_t result = (seq_t)r * pow1 * pow2;
    return result;
}
