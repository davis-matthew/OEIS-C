/*
 * A381831  Expansion of ( (1/x) * Series_Reversion( x * ((1-x) * (1-x+x^2))^3
 *          ) )^(1/3).
 * https://oeis.org/A381831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*n+k+1,k) * binomial(4*n-2*k,n-k)
 *            / (3*n+k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

#if defined(__cplusplus)
/* C++ version – rely on seq_t arithmetic (supports big integer types) */
    seq_t sum = 0;
    seq_t T   = binom((idx_t)(4*n), (idx_t)n);      /* binom(4n,n)      */
    T = T / (seq_t)(3*n + 1);                       /* T0 = binom(4n,n)/(3n+1) */
    sum = T;

    for (idx_t k = 0; k < n; ++k) {
        seq_t num = (seq_t)(n - k) * (seq_t)(3*n - k) * (seq_t)(3*n + k + 1);
        seq_t den = (seq_t)(k + 1) * (seq_t)(4*n - 2*k) * (seq_t)(4*n - 2*k - 1);
        T = (T * num) / den;
        sum += T;
    }
    return sum;
#else
/* C version – use 128‑bit intermediate to avoid overflow for 64‑bit seq_t */
    seq_t sum = 0;
    seq_t T   = binom((idx_t)(4*n), (idx_t)n);
    T = (seq_t)((__int128)T / (seq_t)(3*n + 1));
    sum = T;

    for (idx_t k = 0; k < n; ++k) {
        unsigned __int128 a = (unsigned __int128)(n - k);
        unsigned __int128 b = (unsigned __int128)(3*n - k);
        unsigned __int128 c = (unsigned __int128)(3*n + k + 1);
        unsigned __int128 num = a * b * c;

        unsigned __int128 d = (unsigned __int128)(k + 1);
        unsigned __int128 e = (unsigned __int128)(4*n - 2*k);
        unsigned __int128 f = (unsigned __int128)(4*n - 2*k - 1);
        unsigned __int128 den = d * e * f;

        T = (seq_t)(((__int128)T * num) / den);
        sum += T;
    }
    return sum;
#endif
}
