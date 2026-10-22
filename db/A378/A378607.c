/*
 * A378607  Dirichlet convolution of sigma and the Dirichlet inverse of
 *          A003961 (A346234).
 * https://oeis.org/A378607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Σ_{d|n} σ(d)·A346234(n/d) Equivalent multiplicative form:
 *            a(p^e) = (p^(e+1) - nextprime(p)·(p^e-1) - 1)/(p-1)
 * Range:     i8 n <= 215, i16 n <= 8956, i32 n <= 8956, i64 n <= 8956,
 *            i128 n <= 8956, big any n
 * Helpers:   build_primes_upto, floor_isqrt_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compile with <stdint.h> and the provided utilities. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/next_prime.h"
#include "../utils/floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------------
   Compute a(p^e) using the closed formula.
   All intermediate arithmetic is done in signed 128‑bit
   to avoid overflow for small input ranges.
   ------------------------------------------------------------------ */
static inline seq_t
a_prime_power(uint64_t p, idx_t e)
{
    /* compute p^e and p^(e+1) as __int128 to avoid overflow */
    __int128 pow_e  = 1;
    for (idx_t i = 0; i < e; ++i)           /* p^e */
        pow_e *= p;
    __int128 pow_e1 = pow_e * p;            /* p^(e+1) */

    uint64_t np = next_prime(p);            /* next prime after p */
    __int128 numerator = pow_e1
                        - (__int128)np * (pow_e - 1)
                        - 1;
    __int128 denom     = (int64_t)(p - 1);   /* p>1 always */

    __int128 val128 = numerator / denom;    /* exact division */
    return (seq_t)val128;
}

/* ------------------------------------------------------------------
   Main sequence function.
   The function caches a list of primes up to sqrt(n) so that
   subsequent calls reuse the same table; it works in any call order.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* lazy prime list cache */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    idx_t sqrt_n = floor_isqrt_u64(n);
    if (sqrt_n > prime_limit) {
        build_primes_upto(sqrt_n, &prime_list, &prime_cnt);
        prime_limit = sqrt_n;
    }

    idx_t tmp = n;
    seq_t product = (seq_t)1;

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= sqrt_n; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            product *= a_prime_power(p, exp);
        }
    }

    /* remaining prime factor if any */
    if (tmp > 1) {
        product *= a_prime_power(tmp, 1);
    }

    return product;
}
