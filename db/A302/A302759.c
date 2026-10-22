/*
 * A302759  Number of total dominating sets in the n-Andrásfai graph.
 * https://oeis.org/A302759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8^(n + 1) + (2^n*(n - 2) - 4^(n + 1))*(3*n - 1))/16 - 1,
 *            n > 1 Formula: a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <type_traits>
#endif
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* not used, but guards against zero */

#ifdef __cplusplus
    constexpr bool big_seq = !std::is_integral<seq_t>::value || sizeof(seq_t) > 8;
#else
    /* In C, we assume a built‑in unsigned type (≤ 64 bits).  */
    enum { big_seq = 0 };
#endif

    if (n == 1) return (seq_t)1;

    if (big_seq) {
        /* Use the exact formula with arbitrary‑precision integers. */
        seq_t pow2_n      = pow2(n);                   /* 2^n            */
        seq_t pow2_2np1   = pow2(2 * (n + 1));          /* 4^(n+1)        */
        seq_t pow2_3np1   = pow2(3 * (n + 1));          /* 8^(n+1)        */

        seq_t t  = (seq_t)(3 * n - 1);                 /* (3n‑1)         */
        seq_t y  = pow2_n * (seq_t)(n - 2);            /* 2^n*(n‑2)      */
        y *= t;                                       /* (2^n*(n‑2))*(3n‑1) */
        seq_t z  = pow2_2np1 * t;                     /* 4^(n+1)*(3n‑1)  */

        seq_t sum  = pow2_3np1 + y;                   /* 8^(n+1) + …    */
        seq_t diff = sum - z;                         /* … minus part   */
        seq_t res  = diff >> 4;                       /* divide by 16   */
        res -= (seq_t)1;
        return res;
    } else {
        /* Built‑in unsigned type: compute with 128‑bit intermediates.
           Only works safely for n where the intermediate values
           fit in 128 bits (essentially for n ≤ 21, which covers all
           values that fit in 64 bits). */
        __int128 pow2_n      = 1;
        for (idx_t i = 0; i < n; ++i) pow2_n <<= 1;     /* 2^n */

        __int128 pow2_2np1   = 1;
        for (idx_t i = 0; i < 2 * (n + 1); ++i) pow2_2np1 <<= 1; /* 4^(n+1) */

        __int128 pow2_3np1   = 1;
        for (idx_t i = 0; i < 3 * (n + 1); ++i) pow2_3np1 <<= 1; /* 8^(n+1) */

        __int128 t   = ( __int128 )(3 * n - 1);           /* (3n‑1)         */
        __int128 y   = pow2_n * (n - 2);                 /* 2^n*(n‑2)      */
        y *= t;                                          /* (2^n*(n‑2))*(3n‑1) */
        __int128 z   = pow2_2np1 * t;                     /* 4^(n+1)*(3n‑1)  */

        __int128 sum = pow2_3np1 + y;                    /* 8^(n+1) + …    */
        __int128 diff= sum - z;                          /* … minus part   */
        __int128 res = diff >> 4;                        /* divide by 16   */
        res -= 1;

        return (seq_t)res;
    }
}
