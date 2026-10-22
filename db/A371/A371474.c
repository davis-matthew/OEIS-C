/*
 * A371474  Numbers k such that binomial(k^2,k) == 0 (mod k^3).
 * https://oeis.org/A371474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k where binomial(k^2, k) ≡ 0 (mod k^3)
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   append_candidate, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/append_candidate.h"

static seq_t *seq_vals = NULL;   /* stored sequence values            */
static size_t seq_len   = 0;     /* number of values already found    */
static size_t seq_cap   = 0;     /* current allocated capacity        */
static idx_t  next_k    = 1;     /* next integer to test              */

/* Check if binomial(k^2, k) is divisible by k^3. */
static bool is_good(idx_t k)
{
    uint64_t primes[64], exps[64];
    size_t cnt = 0;

    /* factor k to obtain its prime powers */
    factor_u64((uint64_t)k, primes, exps, &cnt);

    /* k^2 in 128‑bit to avoid overflow for large k */
    __uint128_t k128  = ( __uint128_t)k;
    __uint128_t kk128 = k128 * k128;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p     = primes[i];
        uint64_t ek    = exps[i];
        __uint128_t thr = (__uint128_t)3 * ek;   /* required exponent in binomial */

        __uint128_t exp_binom = 0;
        __uint128_t pow = p;

        while (pow <= kk128) {
            __uint128_t t1 = kk128 / pow;       /* floor(k^2 / p^i) */
            __uint128_t t2 = k128  / pow;       /* floor(k   / p^i) */
            __uint128_t t3 = (kk128 - k128) / pow; /* floor((k^2-k) / p^i) */

            exp_binom += t1 - t2 - t3;
            if (exp_binom >= thr)
                break;  /* sufficient exponent achieved */

            if (pow > kk128 / p)  /* avoid overflow of pow*p */
                break;
            pow *= p;
        }

        if (exp_binom < thr)
            return false;   /* prime p does not satisfy the condition */
    }
    return true;          /* all prime powers satisfied */
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 1 */

    /* Ensure we have computed the first n terms. */
    while (seq_len < n) {
        if (is_good(next_k))
            append_candidate(&seq_vals, &seq_len, &seq_cap, (seq_t)next_k);
        ++next_k;
    }
    return seq_vals[n - 1];
}
