/*
 * A391327  Expansion of g/(1 + x^2*g^6), where g = 1+x*g^4 is the g.f. of
 *          A002293.
 * https://oeis.org/A391327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (-1)^k * (6*k+1) *
 *            binomial(4*n-2*k, n-2*k) / (3*n+1) (Equivalent to the formula in
 *            the OEIS entry)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"          /* provides seq_t binomial(idx_t n, idx_t k) */

/* Local 128‑bit binomial for small seq_t builds (<= 8 bytes). */
static __int128 binom128(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    __int128 r = 1;
    for (idx_t i = 1; i <= k; ++i)
        r = r * (n - k + i) / i;
    return r;
}

seq_t a(idx_t n)
{
    /* For 64‑bit or smaller seq_t, use __int128 intermediates.
       For larger seq_t (arbitrary precision), rely on the provided
       binomial() and seq_t arithmetic. */
    if (sizeof(seq_t) <= 8) {
        __int128 pos = 0, neg = 0;
        idx_t kmax = n / 2;
        for (idx_t k = 0; k <= kmax; ++k) {
            idx_t N = 4 * n - 2 * k;
            idx_t K = n - 2 * k;
            __int128 bin = binom128(N, K);
            __int128 term = bin * (6 * k + 1);
            if ((k & 1) == 0)
                pos += term;
            else
                neg += term;
        }
        __int128 sum = pos - neg;
        sum /= (__int128)(3 * n + 1);
        return (seq_t)sum;
    } else {
        seq_t pos = 0, neg = 0;
        idx_t kmax = n / 2;
        for (idx_t k = 0; k <= kmax; ++k) {
            idx_t N = 4 * n - 2 * k;
            idx_t K = n - 2 * k;
            seq_t bin = binomial(N, K);
            seq_t term = bin * (seq_t)(6 * k + 1);
            if ((k & 1) == 0)
                pos += term;
            else
                neg += term;
        }
        seq_t sum = pos - neg;
        sum = sum / (seq_t)(3 * n + 1);
        return sum;
    }
}
