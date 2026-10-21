/*
 * A253286  Square array read by upward antidiagonals, A(n,k) = Sum_{j=0..n}
 *          (n-j)!*C(n,n-j)* C(n-1,n-j)*k^j, for n>=0 and k>=0.
 * https://oeis.org/A253286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, easy, nonn
 * Formula:   a(n) is the entry of the square array A(n,k) read by upward
 *            antidiagonals. The square array satisfies A(0,k) = 1 A(1,k) = k
 *            A(n,k) = (2*n + k - 2)*A(n-1,k) - (n-1)*(n-2)*A(n-2,k) for n > 1
 *            The sequence a(n) is obtained by reading A(n,k) in order of
 *            increasing n+k (upward antidiagonals)
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 91, u64 n <= 231,
 *            u128 n <= 562, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root, no overflow (returns floor(sqrt(x))) */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = (1ULL << 32);           /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Compute A(n,k) with the recurrence shown above.  n and k are non‑negative. */
static inline seq_t A_calc(idx_t n, idx_t k)
{
    if (n == 0) return (seq_t)1;
    seq_t T0 = (seq_t)1;        /* A(0,k) */
    seq_t T1 = (seq_t)k;        /* A(1,k) */
    if (n == 1) return T1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t mul1 = (seq_t)(2 * i + k - 2);          /* (2n + k - 2) */
        seq_t mul2 = (seq_t)((i - 1) * (i - 2));      /* (n-1)*(n-2) */
        seq_t temp  = mul1 * T1 - mul2 * T0;
        T0 = T1;
        T1 = temp;
    }
    return T1;
}

/* Main sequence function.  The index n is the position in the upward
   antidiagonal ordering. */
seq_t a(idx_t n)
{
    /* Find the diagonal d such that n is in that diagonal. */
    uint64_t t = n * 8ULL + 1;                 /* 8*n + 1 */
    uint64_t s = isqrt_uint64(t);              /* floor(sqrt(8n+1)) */
    uint64_t d = (s - 1) / 2;                  /* diagonal index */

    /* Compute the start index of this diagonal: t(d-1) = d*(d+1)/2 */
    uint64_t start = d * (d + 1) / 2;

    /* Position within the diagonal: p = n - start, 0 ≤ p ≤ d */
    uint64_t p = n - start;

    /* Corresponding n' and k for A(n',k) */
    uint64_t n_prime = d - p;
    uint64_t k       = p;

    return A_calc(n_prime, k);
}
