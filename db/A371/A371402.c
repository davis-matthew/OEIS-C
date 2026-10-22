/*
 * A371402  a(n) = gcd(2*n, 4^n)^(2*n + 1) mod (2^(2*n + 1) - 1)^2.
 * https://oeis.org/A371402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(2*n, 4^n)^(2*n + 1) mod (2^(2*n + 1) - 1)^2 as
 *            defined for n ≥ 0. a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 15,
 *            big any n
 * Helpers:   mod_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/mod_pow.h"

/* Count trailing zeros of an unsigned integer n (n > 0). */
static idx_t trailing_zeros(idx_t n)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_ctzll(n);
#else
    idx_t cnt = 0;
    while ((n & 1) == 0) {
        ++cnt;
        n >>= 1;
    }
    return cnt;
#endif
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* a(0) = 0 by definition. */
    if (n == 0)
        return (seq_t)0;

    /* Exponent of 2 in the gcd: 1 + v2(n). */
    idx_t v2   = trailing_zeros(n);
    idx_t exp  = (1 + v2) * (2 * n + 1);     /* exponent of the base 2 */
    idx_t T    = 2 * n + 1;                   /* exponent for the modulus base */

    /* When seq_t is 64‑bit (or smaller), M and M^2 fit in 128 bits for all n
       whose answer fits in seq_t, so we can use __uint128_t arithmetic. */
    if (sizeof(seq_t) <= 8) {
        /* M = 2^T - 1.  T ≤ 61 for all n whose a(n) fits in 64 bits. */
        unsigned __int128 M   = (((unsigned __int128)1) << T) - 1;
        unsigned __int128 mod = M * M;   /* Modulus M^2: fits in 128 bits */

        unsigned __int128 res = 1;
        /* Compute 2^exp mod mod by repeated doubling. */
        for (idx_t i = 0; i < exp; ++i) {
            res <<= 1;
            if (res >= mod)
                res -= mod;
        }
        return (seq_t)res;
    }

    /* For arbitrary‑precision seq_t, use the provided utilities. */
    seq_t M    = pow2(T) - (seq_t)1;  /* 2^T - 1  */
    seq_t modSq = M * M;              /* (2^T - 1)^2  */
    return mod_pow((seq_t)2, exp, modSq);
}
