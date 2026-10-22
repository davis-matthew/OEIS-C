/*
 * A369507  Expansion of (1/x) * Series_Reversion( x / ((1+x)^3+x^3)^2 ).
 * https://oeis.org/A369507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} * binomial(2n+2, k) *
 *            binomial(6n-3k+6, n-3k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Detect whether the sequence type is small (≤64 bits). */
#if defined(__cplusplus)
    constexpr bool small_seq_t = (sizeof(seq_t) <= 8ULL);
#else
    static const int small_seq_t = (sizeof(seq_t) <= 8ULL);
#endif

/* 128‑bit binomial for small types – uses only 128‑bit intermediates. */
static inline unsigned __int128 binom128(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (unsigned __int128)(n - k + i);
        res /= (unsigned __int128)i;
    }
    return res;
}

/* Compute a(n) when seq_t is small (≤64 bits). */
static inline seq_t a_small(idx_t n)
{
    unsigned __int128 sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        unsigned __int128 b1 = binom128(2 * n + 2, k);
        unsigned __int128 b2 = binom128(6 * n - 3 * k + 6, n - 3 * k);
        sum += b1 * b2;
    }
    unsigned __int128 res = sum / (unsigned __int128)(n + 1);
    return (seq_t)res;
}

/* Compute a(n) for large or big‑int seq_t (uses the provided binomial). */
static inline seq_t a_big(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t b1 = binomial(2 * n + 2, k);
        seq_t b2 = binomial(6 * n - 3 * k + 6, n - 3 * k);
        seq_t prod = b1 * b2;
        sum = sum + prod;
    }
    seq_t denom = (seq_t)(n + 1);
    return sum / denom;
}

/* Public entry point – calls the appropriate routine depending on seq_t size. */
seq_t a(idx_t n)
{
#if defined(__cplusplus)
    if constexpr (small_seq_t)
        return a_small(n);
    else
        return a_big(n);
#else
    if (small_seq_t)
        return a_small(n);
    else
        return a_big(n);
#endif
}
