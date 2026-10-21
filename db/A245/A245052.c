/*
 * A245052  Number of hybrid 9-ary trees with n internal nodes.
 * https://oeis.org/A245052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(8*n+1) * Sum_{i=0..n} C(8*n+i,i) * C(8*n+i+1,n-i)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses 128‑bit arithmetic for 64‑bit seq_t types to avoid
   intermediate overflow, otherwise it relies on seq_t operations directly. */

#include "../utils/binom.h"

/* Helper to determine if seq_t fits in 8 bytes (usual 64‑bit natural type). */
#if defined(__GNUC__) || defined(__clang__)
    #define USE_INT128_FOR_SMALL_TYPES 1
#else
    #define USE_INT128_FOR_SMALL_TYPES 0
#endif

seq_t a(idx_t n)
{
    /* The division factor. */
    idx_t denom = 8 * n + 1;

#if USE_INT128_FOR_SMALL_TYPES
    /* If seq_t is a 64‑bit (or smaller) unsigned integer, perform computations
       in unsigned __int128 to preserve exactness of intermediate values. */
    if (sizeof(seq_t) <= 8)
    {
        unsigned __int128 sum = 0;
        idx_t i;
        for (i = 0; i <= n; ++i)
        {
            seq_t c1 = binom(8 * n + i, i);
            seq_t c2 = binom(8 * n + i + 1, n - i);

            unsigned __int128 prod = (unsigned __int128)c1 * (unsigned __int128)c2;
            sum += prod;
        }
        return (seq_t)(sum / denom);
    }
#endif

    /* For arbitrary‑precision seq_t or when the 128‑bit branch isn’t taken,
       perform the calculation directly with seq_t arithmetic. */
    seq_t sum = 0;
    idx_t i;
    for (i = 0; i <= n; ++i)
    {
        seq_t c1 = binom(8 * n + i, i);
        seq_t c2 = binom(8 * n + i + 1, n - i);
        sum += c1 * c2;
    }
    return sum / denom;
}
