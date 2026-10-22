/*
 * A365132  G.f. A(x) satisfies A(x) = (1 + x*A(x)*(1 + x*A(x))^3)^3.
 * https://oeis.org/A365132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..n} C(3*(n+1), k) * C(3k, n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The closed‑form sum is valid for all n ≥ 0 with offset 0. */
    if (n == 0)
        return (seq_t)1;

    idx_t np1 = n + 1;
    idx_t three_np1 = 3 * np1;   /* 3*(n+1) */

    /* For C compilation (builtin integer types) use an unsigned __int128
       accumulator to avoid overflow; for C++ compilation (big‑integer class)
       use seq_t itself. */
#ifdef __cplusplus
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t c1 = binomial(three_np1, k);   /* C(3*(n+1), k) */
        seq_t c2 = binomial(3 * k, n - k);   /* C(3k, n-k)   */
        sum += c1 * c2;
    }
    return sum / np1;
#else
    unsigned __int128 sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t c1 = binomial(three_np1, k);
        seq_t c2 = binomial(3 * k, n - k);
        sum += (unsigned __int128)c1 * c2;
    }
    return (seq_t)(sum / np1);
#endif
}
