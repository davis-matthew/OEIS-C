/*
 * A370124  a(0) = 0, a(1) = 1, and for n > 1, a(n) = 1 if the least prime
 *          dividing the arithmetic derivative of n is equal to the least
 *          prime not dividing n, otherwise a(n) = 0.
 * https://oeis.org/A370124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the least prime dividing the arithmetic derivative
 *            of n equals the least prime not dividing n, otherwise 0. The
 *            function works for any n ≥ 0 and is efficient for n ≤ 10 000
 * Range:     u8 n <= 9474, u16 n <= 9474, u32 n <= 9474, u64 n <= 9474,
 *            u128 n <= 9474, big any n
 * Helpers:   _powmod_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/_powmod_uint64.h"

/* --------------------------------------------------------------------- *
 *  Prime sieve (up to 200 000 – sufficient for n ≤ 10 000)              *
 * --------------------------------------------------------------------- */
#define _PRIME_MAX 200000

static uint8_t  _prime_sieve[_PRIME_MAX + 1];
static uint64_t _prime_list[_PRIME_MAX];
static int      _prime_list_cnt = 0;
static bool     _prime_list_inited = false;

static void _init_prime_list(void)
{
    if (_prime_list_inited) return;
    for (int i = 2; i <= _PRIME_MAX; ++i) _prime_sieve[i] = 1;
    for (int p = 2; (long long)p * p <= _PRIME_MAX; ++p)
        if (_prime_sieve[p])
            for (int j = p * p; j <= _PRIME_MAX; j += p)
                _prime_sieve[j] = 0;
    int idx = 0;
    for (int i = 2; i <= _PRIME_MAX; ++i)
        if (_prime_sieve[i])
            _prime_list[idx++] = (uint64_t)i;
    _prime_list_cnt = idx;
    _prime_list_inited = true;
}

/* --------------------------------------------------------------------- *
 *  Factorization of n: primes and exponents                             *
 * --------------------------------------------------------------------- */
static void _factor_uint64(uint64_t n, uint64_t *primes, int *exps, int *cnt)
{
    int c = 0;
    uint64_t tmp = n;
    _init_prime_list();
    for (int i = 0; i < _prime_list_cnt && _prime_list[i] * _prime_list[i] <= tmp; ++i) {
        uint64_t p = _prime_list[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[c] = p;
            exps[c]   = e;
            ++c;
        }
    }
    if (tmp > 1) {
        primes[c] = tmp;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
}

/* --------------------------------------------------------------------- *
 *  Least prime not dividing n                                            *
 * --------------------------------------------------------------------- */
static uint64_t _least_prime_not_dividing(uint64_t n)
{
    _init_prime_list();
    for (int i = 0; i < _prime_list_cnt; ++i) {
        uint64_t p = _prime_list[i];
        if (n % p != 0) return p;
    }
    /* This point is reached only for extremely large n; return next prime */
    return _prime_list[_prime_list_cnt - 1] + 2;
}

/* --------------------------------------------------------------------- *
 *  Least prime dividing the arithmetic derivative of n                   *
 * --------------------------------------------------------------------- */
static uint64_t _least_prime_factor_arith_derivative(uint64_t n)
{
    if (n <= 1) return 0;   /* derivative of 1 is 0 → treat as 0 */

    _init_prime_list();

    /* factor n */
    uint64_t fac_primes[64];
    int fac_exps[64];
    int fac_cnt = 0;
    _factor_uint64(n, fac_primes, fac_exps, &fac_cnt);

    /* test candidate primes in ascending order */
    for (int idx = 0; idx < _prime_list_cnt; ++idx) {
        uint64_t p = _prime_list[idx];
        /* compute derivative mod p */
        uint64_t d_mod = 0;
        for (int i = 0; i < fac_cnt; ++i) {
            uint64_t pi = fac_primes[i];
            uint64_t ei = fac_exps[i];
            uint64_t term = _powmod_uint64(pi % p, ei - 1, p);   /* pi^(ei-1) mod p */
            uint64_t prod_except = 1 % p;
            for (int j = 0; j < fac_cnt; ++j) if (j != i) {
                prod_except = (prod_except *
                               _powmod_uint64(fac_primes[j] % p,
                                              fac_exps[j], p)) % p;
            }
            term = (ei % p) * term % p;
            term = term * prod_except % p;
            d_mod = (d_mod + term) % p;
        }
        if (d_mod == 0) return p;   /* found least prime divisor */
    }
    /* No divisor found within prime list – treat as 0 (no prime factor) */
    return 0;
}

/* --------------------------------------------------------------------- *
 *  Main sequence function                                                *
 * --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    uint64_t n64 = (uint64_t)n;

    uint64_t lpf_deriv = _least_prime_factor_arith_derivative(n64);
    uint64_t lpn_notdiv = _least_prime_not_dividing(n64);

    if (lpf_deriv == lpn_notdiv) return 1;
    return 0;
}
