/*
 * A365753  Expansion of (1/x) * Series_Reversion( x*(1+x)*(1-x)^5 ).
 * https://oeis.org/A365753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..n} (-1)^k * binomial(n+k,k) * *
 *            binomial(6*n - k + 4, n-k) (OEIS A365753)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Helper for small seq_t (≤ 64 bits) using 128‑bit intermediate */
static inline unsigned __int128 _binom128(idx_t N, idx_t K)
{
    if (K > N - K) K = N - K;
    unsigned __int128 r = 1;
    for (idx_t i = 1; i <= K; ++i)
    {
        r = r * (N - K + i);
        r /= i;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Special case for n = 0 (avoids division by zero in routines) */
    if (n == 0) return (seq_t)1;

#if (defined(__SIZEOF_INT128__) && (defined(__GNUC__) || defined(__clang__))) || (defined(_MSC_VER) && !defined(_MSVC_STL_VERSION))
    /* Assume seq_t is at most 64 bits; use 128‑bit intermediate. */
    unsigned __int128 pos = 0, neg = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        unsigned __int128 t1 = _binom128(n + k, k);
        idx_t m = 6 * n - k + 4;
        unsigned __int128 t2 = _binom128(m, n - k);
        unsigned __int128 prod = t1 * t2;
        if ((k & 1) == 0) pos += prod; else neg += prod;
    }
    unsigned __int128 diff = pos - neg;          /* diff ≥ 0 */
    unsigned __int128 res  = diff / (unsigned __int128)(n + 1);
    return (seq_t)res;
#else
    /* seq_t is arbitrary precision; use provided binom routine. */
    seq_t pos = (seq_t)0, neg = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t t1 = binom(n + k, k);
        idx_t m = 6 * n - k + 4;
        seq_t t2 = binom(m, n - k);
        seq_t prod = t1 * t2;
        if ((k & 1) == 0) pos += prod; else neg += prod;
    }
    seq_t diff = pos - neg;        /* assume pos ≥ neg for this sequence */
    seq_t denom = (seq_t)(n + 1);
    return diff / denom;
#endif
}
