/*
 * A365847  Expansion of (1/x) * Series_Reversion( x*(1-x)^4/(1+x)^4 ).
 * https://oeis.org/A365847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..n} binomial(4*n + k + 3, k) *
 *            binomial(4*(n+1), n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses 128‑bit arithmetic for the intermediate
    binomial products and the accumulating sum.  The final result is
    cast back to seq_t, which is guaranteed to hold the value whenever
    the caller’s seq_t type can represent it. */

#include <stdint.h>

/* 128‑bit integer type: works with GCC/Clang on both C11 and C++17. */
#if defined(__GNUC__) || defined(__clang__)
typedef unsigned __int128 uint128_t;
#else
#   error "128‑bit integers are required for this implementation."
#endif

/* Compute binomial(n,k) using multiplicative formula,
   with the result returned as a 128‑bit integer. */
static inline uint128_t binom128(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    uint128_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res /= i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /*  Compute the sum of the product of binomials for each k.  */
    uint128_t sum = 0;
    idx_t m = 4 * (n + 1);      /* 4*(n+1) appears in the second binomial. */
    for (idx_t k = 0; k <= n; ++k) {
        uint128_t bin1 = binom128(4 * n + k + 3, k);
        uint128_t bin2 = binom128(m, n - k);
        sum += bin1 * bin2;
    }

    /*  Divide by (n+1) to obtain the final term.  The division
        is exact for the formula.  */
    uint128_t result128 = sum / (n + 1);
    return (seq_t)result128;
}
