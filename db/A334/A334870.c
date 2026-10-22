/*
 * A334870  If n is a square, a(n) = A000196(n), and for nonsquare n, let p be
 *          the smallest prime dividing the squarefree part of n. Divide n by
 *          p and multiply by the product of all smaller primes.
 * https://oeis.org/A334870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000196(n) if n is a perfect square; otherwise * let m be
 *            the square‑free part of n, p the smallest prime * dividing m;
 *            then a(n) = (n / p) * (∏_{q<p} q) , * where the product is taken
 *            over all primes q < p. * (This implementation follows the OEIS
 *            description of A334870.)
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 30, u64 n <= 58, u128 n <= 106,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"   /* provides build_primes_upto() */

static uint64_t *primes = NULL;   /* primes up to current limit */
static size_t    prime_cnt = 0;
static uint64_t  prime_limit = 0;

/* enlarge the prime table to include all primes ≤ limit */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= prime_limit)
        return;
    /* free old table */
    free(primes);
    primes = NULL;
    prime_cnt = 0;
    /* build new table */
    build_primes_upto(limit, &primes, &prime_cnt);
    prime_limit = limit;
}

/* integer square root of a 64–bit unsigned integer */
static uint64_t integer_sqrt_uint64(uint64_t n)
{
    uint64_t r = (uint64_t)sqrt((long double)n);
    while ((r + 1) <= (UINT64_MAX / (r + 1)) && (r + 1) * (r + 1) <= n)
        ++r;
    while (r * r > n)
        --r;
    return r;
}

/* smallest prime factor of n (n ≥ 2), using the global prime table
 * which must contain all primes ≤ sqrt(n) */
static uint64_t smallest_prime_factor_uint64(uint64_t n)
{
    if (n <= 1)
        return n;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= n; ++i)
        if (n % primes[i] == 0)
            return primes[i];
    return n;  /* n is prime */
}

/* main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined, harmless */

    uint64_t nn = (uint64_t)n;
    uint64_t sq = integer_sqrt_uint64(nn);
    if (sq * sq == nn)                /* perfect square */
        return (seq_t)sq;

    /* ensure primes up to sqrt(nn) for factorisation */
    uint64_t limit = integer_sqrt_uint64(nn);
    ensure_primes_up_to(limit);

    /* compute the square‑free part m of nn */
    uint64_t m = nn;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > m)
            break;
        while (m % (p * p) == 0)
            m /= p * p;
    }

    /* m is now the square‑free part; it is >1 because nn is not a square */
    if (m == 1) {    /* should not happen, but guard nevertheless */
        return (seq_t)1;
    }

    /* find smallest prime factor p of m */
    uint64_t sqrt_m = integer_sqrt_uint64(m);
    ensure_primes_up_to(sqrt_m);
    uint64_t p = smallest_prime_factor_uint64(m);

    /* ensure primes up to p-1 for the primorial */
    if (p > 1)
        ensure_primes_up_to(p - 1);

    /* compute primorial of all primes < p */
    seq_t primorial = (seq_t)1;
    for (size_t i = 0; i < prime_cnt && primes[i] < p; ++i)
        primorial = primorial * (seq_t)primes[i];

    /* final result: (nn / p) * primorial */
    seq_t result = (seq_t)(nn / p) * primorial;
    return result;
}
