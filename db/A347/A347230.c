/*
 * A347230  Möbius transform of A344695, gcd(sigma(n), psi(n)).
 * https://oeis.org/A347230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} μ(n/d) * A344695(d) where A344695(d) =
 *            gcd(σ(d), ψ(d)) σ(d) is the sum-of-divisors function, ψ(d) = Π
 *            p^{e-1}·(p+1) over prime powers p^e || d A344695 is not
 *            multiplicative; we cache its values for reuse
 * Range:     i8 n <= 128, i16 n <= 9897, i32 n <= 9897, i64 n <= 9897,
 *            i128 n <= 9897, big any n
 * Helpers:   gcd, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*---------------------------------------------------------------*/
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/mobius.h"

/* -----------------------------------------------------------------
   Local helpers
----------------------------------------------------------------- */
/* Greatest‑common divisor for __int128 operands */
static inline __int128 gcd128(__int128 a, __int128 b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute A344695(n) = gcd(sigma(n), psi(n)) using 128‑bit arithmetic */
static seq_t compute_A344695(idx_t n) {
    __int128 sigma = 1;
    __int128 psi   = 1;
    uint64_t m     = (uint64_t)n;

    /* prime factorisation by simple trial division */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            size_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            /* sigma factor: (p^(e+1)-1)/(p-1) */
            __int128 p_pow = 1;
            for (size_t i = 0; i < e + 1; ++i) p_pow *= (__int128)p;
            __int128 sigma_factor = (p_pow - 1) / (__int128)(p - 1);
            sigma *= sigma_factor;

            /* psi factor: p^(e-1)*(p+1) */
            __int128 p_pow_e_minus1 = 1;
            for (size_t i = 0; i < e - 1; ++i) p_pow_e_minus1 *= (__int128)p;
            __int128 psi_factor = p_pow_e_minus1 * ((__int128)p + 1);
            psi *= psi_factor;
        }
    }

    if (m > 1) {               /* remaining prime factor with e = 1 */
        __int128 p = (__int128)m;
        __int128 sigma_factor = (p * p - 1) / (__int128)(p - 1); /* 1 + p   */
        sigma *= sigma_factor;
        __int128 psi_factor = (__int128)(p + 1);
        psi *= psi_factor;
    }

    __int128 g = gcd128(sigma, psi);
    return (seq_t)g;          /* cast to the user’s integer type */
}

/* --------------------------------------------------------------- */
/* Cached values of A344695 */
static seq_t *A_cache = NULL;
static idx_t   A_cache_cap = 0;

/* Grow the cache to contain values up to n (inclusive) */
static void ensure_A_cache(idx_t n) {
    if (n <= A_cache_cap) return;
    size_t new_cap = (size_t)n + 1;                   /* index 0 … n */
    seq_t *tmp = (seq_t *)realloc(A_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                               /* allocation failure */
    A_cache = tmp;

    for (idx_t i = A_cache_cap + 1; i <= n; ++i)
        A_cache[i] = compute_A344695(i);

    A_cache_cap = n;
}

/* --------------------------------------------------------------- */
/* Public sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;                            /* out of offset */
    ensure_A_cache(n);

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;

            seq_t mu1 = mobius(e);                  /* μ(n/d) */
            sum += mu1 * A_cache[d];

            if (e != d) {
                seq_t mu2 = mobius(d);              /* μ(n/e) = μ(d) */
                sum += mu2 * A_cache[e];
            }
        }
    }
    return sum;
}
