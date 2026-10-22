/*
 * A369006  a(n) = 1 if there is no prime p such that p^p divides n' /
 *          gcd(n,n'), and 0 otherwise, where n' stands for the arithmetic
 *          derivative of n, A003415(n).
 * https://oeis.org/A369006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if no prime p has p^p | n' / gcd(n,n'), else 0 * (n' is
 *            the arithmetic derivative of n)
 * Range:     u8 n <= 9364, u16 n <= 9364, u32 n <= 9364, u64 n <= 9364,
 *            u128 n <= 9364, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/factor_u64.h"    /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

/* Euclidean GCD for __int128 values (used for large n') */
static inline __int128 gcd128(__int128 a, __int128 b) {
    while (b != 0) {
        __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute the arithmetic derivative n' of a 64‑bit integer n.
 * Uses the formula n' = Σ e_i * n / p_i, where n = ∏ p_i^e_i. */
static __int128 arithmetic_derivative(idx_t n) {
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    __int128 nval = n;          /* use 128‑bit to avoid overflow */
    __int128 der = 0;
    for (size_t i = 0; i < cnt; ++i) {
        __int128 term = (__int128)exps[i] * nval / (__int128)primes[i];
        der += term;
    }
    return der;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 1)
        return 0;                 /* a(1) = 0 by definition */

    /* 1. arithmetic derivative n' (128‑bit) */
    __int128 nprime128 = arithmetic_derivative(n);

    /* 2. GCD of n and n' (128‑bit) */
    __int128 nix = n;                      /* n fits in 64‑bit, cast to 128‑bit */
    __int128 g128 = gcd128(nix, nprime128);

    /* 3. m = n' / gcd(n,n') */
    __int128 m128 = nprime128 / g128;
    if (m128 <= 0)
        return 0;                         /* should not happen except for n=1 */

    /* 4. Ensure m fits in 64‑bit for the factorization routine.
     * For the intended use (n up to ~10^4) this holds. */
    uint64_t m = (uint64_t)m128;

    /* 5. Factor m and test whether any prime exponent >= the prime itself */
    uint64_t fpr[64], fex[64];
    size_t fcnt = 0;
    factor_u64(m, fpr, fex, &fcnt);

    for (size_t i = 0; i < fcnt; ++i) {
        if (fex[i] >= fpr[i])          /* p^p | m */
            return 0;
    }
    return 1;
}
