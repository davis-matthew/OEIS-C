/*
 * A323229  a(n) = binomial(2*n, n+1) + 1.
 * https://oeis.org/A323229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n, n+1) + 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute binomial coefficient for non‑negative integers
   using an algorithm that avoids overflow on integer types.  In C
   compilation it uses unsigned __int128 for intermediate values; in
   C++ compilation it relies on built‑in or user‑supplied operators. */
static seq_t binom_u64(idx_t n, idx_t k)
{
#ifdef __cplusplus
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    return res;
#else
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        __uint128_t temp = (__uint128_t)res * (n - k + i);
        temp /= i;
        res = (seq_t)temp;
    }
    return res;
#endif
}

seq_t a(idx_t n)
{
    /* a(n) = C(2*n, n+1) + 1 */
    return binom_u64(n + n, n + (idx_t)1) + (seq_t)1;
}
