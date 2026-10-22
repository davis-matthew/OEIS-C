/*
 * A369014  Expansion of (1/x) * Series_Reversion( x * (1-x^3/(1-x))^3 ).
 * https://oeis.org/A369014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binomial(3*n+k+2,k) *
 *            binomial(n-2*k-1,n-3*k)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 41, u128 n <= 76,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* Compute binomial with proper handling of the special
   case n = 0, k = 0 when the first argument would be -1.  */
static inline seq_t safe_binom(idx_t a, idx_t b)
{
    if (a == (idx_t)-1 && b == 0) /* binomial(-1,0) = 1 */
        return 1;
    if (a >= b && b >= 0)
        return binom(a, b);
    return 0;
}

seq_t a(idx_t n)
{
    /* Handle n = 0 directly to avoid negative indices.  */
    if (n == 0)
        return 1;

    /* Sum over k.  */
#ifdef __cplusplus
    /* C++ compilation – seq_t is likely an arbitrary‑precision
       integer class.  Let the member operators handle large values. */
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term1 = binom(3 * n + k + 2, k);
        idx_t a2 = n - 2 * k - 1;
        idx_t b2 = n - 3 * k;
        seq_t term2 = safe_binom(a2, b2);
        sum += term1 * term2;
    }
    return sum / (n + 1);

#else
    /* C compilation – use 128‑bit arithmetic to avoid overflow on
       integer builds (uint8_t … uint64_t, unsigned __int128).  */
    __uint128_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        __uint128_t term1 = (__uint128_t)binom(3 * n + k + 2, k);
        idx_t a2 = n - 2 * k - 1;
        idx_t b2 = n - 3 * k;
        __uint128_t term2 = (__uint128_t)safe_binom(a2, b2);
        sum += term1 * term2;
    }
    return (seq_t)(sum / (n + 1));
#endif
}
