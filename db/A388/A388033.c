/*
 * A388033  a(n) = gcd(A276086(sigma(n)), A276086(3*n)), where A276086 is the
 *          primorial base exp-function, and sigma is the sum of divisors
 *          function.
 * https://oeis.org/A388033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A276086(sigma(n)), A276086(3*n))
 * Range:     u8 n <= 16, u16 n <= 65, u32 n <= 475, u64 n <= 6005,
 *            u128 n <= 9975, big any n
 * Helpers:   build_primes_upto, factor_u64, gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute the primorial‐base digits for sigma(n) and 3*n,
   take the minimum exponent for each prime, and multiply the
   corresponding prime powers.  No large intermediate
   numbers are formed, so the function works with seq_t of any size. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/build_primes_upto.h"

/* ---------- Global prime table ---------- */
static uint64_t *PRIMES = NULL;
static size_t   PRIMES_CNT = 0;
static bool     PRIMES_INIT = false;

/* ---------- Initialise a list of primes below LIMIT. ---------- */
static void init_primes(void)
{
    if (!PRIMES_INIT) {
        /* 200000 is more than enough for all n used by the tests. */
        const idx_t LIMIT = 200000ULL;
        build_primes_upto(LIMIT, &PRIMES, &PRIMES_CNT);
        PRIMES_INIT = true;
    }
}

/* ---------- sigma(n) as seq_t (sum of divisors). ---------- */
static seq_t sigma_seq(idx_t n)
{
    if (n == 0) return 0;

    /* factor_u64 returns primes in ascending order */
    uint64_t pr[64], ex[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base  = (seq_t)pr[i];
        seq_t power = seq_pow(base, (idx_t)(ex[i] + 1));
        seq_t denom = (seq_t)(pr[i] - 1);
        seq_t term  = (power - 1) / denom;
        sigma *= term;
    }
    return sigma;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    init_primes();

    /* compute both arguments of A276086 */
    const seq_t sigma_n  = sigma_seq(n);
    const seq_t three_n  = (seq_t)n * 3;

    /* Allocate arrays holding the digits of sigma(n) and 3*n in primorial base. */
    size_t *exp_sigma = (size_t *)malloc(PRIMES_CNT * sizeof(size_t));
    size_t *exp_3n    = (size_t *)malloc(PRIMES_CNT * sizeof(size_t));
    if (!exp_sigma || !exp_3n) {
        /* If allocation fails, fall back to an error value. */
        if (exp_sigma) free(exp_sigma);
        if (exp_3n)    free(exp_3n);
        return 0;
    }

    seq_t rem1 = sigma_n;
    seq_t rem2 = three_n;
    seq_t mult = 1;

    size_t i;
    for (i = 0; i < PRIMES_CNT; ++i) {
        uint64_t p = PRIMES[i];
        idx_t   a1 = 0, a2 = 0;

        if (rem1 > 0)
            a1 = (idx_t)((rem1 / mult) % p);
        if (rem2 > 0)
            a2 = (idx_t)((rem2 / mult) % p);

        exp_sigma[i] = (size_t)a1;
        exp_3n[i]    = (size_t)a2;

        if (a1 > 0 || a2 > 0) {
            rem1 -= (seq_t)a1 * mult;
            rem2 -= (seq_t)a2 * mult;
        }

        mult *= p;

        if (rem1 == 0 && rem2 == 0)
            break;
    }
    /* The remaining primes (if any) have all zero digits – no effect. */

    /* Compute the product of primes^min(exp1,exp2). */
    seq_t result = 1;
    for (size_t j = 0; j <= i; ++j) {
        size_t m = exp_sigma[j] < exp_3n[j] ? exp_sigma[j] : exp_3n[j];
        if (m > 0) {
            result *= seq_pow((seq_t)PRIMES[j], (idx_t)m);
        }
    }

    free(exp_sigma);
    free(exp_3n);
    return result;
}
