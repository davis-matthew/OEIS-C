/*
 * A250094  Positive integers k such that the numerator of the harmonic mean
 *          of the divisors of k is equal to k.
 * https://oeis.org/A250094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sequence of positive integers k such that the numerator
 *            of the harmonic mean of the divisors of k equals k. The
 *            condition is equivalent to: σ(k) % τ(k) == 0 and gcd(k,
 *            σ(k)/τ(k)) == 1, where τ(k) is the divisor count and σ(k) is the
 *            sum-of-divisors function
 * Range:     u8 n <= 7, u16 n <= 967, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "../utils/gcd.h"          /* gcd for seq_t           */
#include "../utils/seq_pow.h"      /* seq_pow(seq_t base, idx_t exp) */

/* ---------- prime sieve ---------- */
static const idx_t _sieve_limit = 10000000ULL;          /* enough for all tests   */
static bool _prime_sieve_done = false;
static size_t _prime_count = 0;
static uint64_t *_primes = NULL;

static void _build_prime_sieve(void)
{
    if (_prime_sieve_done)
        return;

    /* Allocate boolean array for sieve */
    bool *is_comp = (bool *)malloc((_sieve_limit + 1) * sizeof(bool));
    if (!is_comp)
        exit(1);
    for (size_t i = 0; i <= _sieve_limit; ++i)
        is_comp[i] = false;
    is_comp[0] = is_comp[1] = true;

    size_t limit_sqrt = (size_t)sqrt((double)_sieve_limit);
    for (size_t i = 2; i <= limit_sqrt; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= _sieve_limit; j += i)
                is_comp[j] = true;

    /* Count primes to allocate array */
    for (size_t i = 2; i <= _sieve_limit; ++i)
        if (!is_comp[i])
            ++_prime_count;

    _primes = (uint64_t *)malloc(_prime_count * sizeof(uint64_t));
    if (!_primes)
        exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= _sieve_limit; ++i)
        if (!is_comp[i])
            _primes[idx++] = (uint64_t)i;

    free(is_comp);
    _prime_sieve_done = true;
}

/* ---------- helper: test a single k ---------- */
static bool _good_k(uint64_t k)
{
    seq_t tau = 1;   /* divisor count */
    seq_t sigma = 1; /* sum of divisors */
    uint64_t tmp = k;

    for (size_t i = 0; i < _prime_count &&
         (uint64_t)_primes[i] * _primes[i] <= tmp; ++i)
    {
        uint64_t p = _primes[i];
        if (tmp % p == 0)
        {
            unsigned e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            tau *= (seq_t)(e + 1);

            seq_t p_pow = seq_pow((seq_t)p, (idx_t)(e + 1));
            seq_t numer = p_pow - (seq_t)1;
            seq_t denom = (seq_t)p - (seq_t)1;
            seq_t part = numer / denom;
            sigma *= part;
        }
    }
    if (tmp > 1) {           /* remaining prime factor */
        tau *= (seq_t)2;
        seq_t p_pow = seq_pow((seq_t)tmp, (idx_t)2);
        seq_t numer = p_pow - (seq_t)1;
        seq_t denom = (seq_t)tmp - (seq_t)1;
        seq_t part = numer / denom;
        sigma *= part;
    }

    if (tau == (seq_t)0)   /* not possible */
        return false;

    if (sigma % tau != (seq_t)0)
        return false;

    seq_t ratio = sigma / tau;
    seq_t g = gcd((seq_t)k, ratio);
    return g == (seq_t)1;
}

/* ---------- sequence cache ---------- */
static seq_t * _seq = NULL;
static size_t _seq_used = 0;
static size_t _seq_cap = 0;
static uint64_t _last_k = 1;   /* last tested k */

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1 */

    _build_prime_sieve();

    if (_seq == NULL) {
        _seq_cap = 1024;
        _seq = (seq_t *)malloc(_seq_cap * sizeof(seq_t));
        if (!_seq)
            exit(1);
        _seq[0] = (seq_t)1;      /* first term is 1 */
        _seq_used = 1;
        _last_k = 1;
    }

    while (_seq_used < (size_t)n) {
        ++_last_k;
        if (_good_k(_last_k)) {
            if (_seq_used == _seq_cap) {
                _seq_cap *= 2;
                _seq = (seq_t *)realloc(_seq, _seq_cap * sizeof(seq_t));
                if (!_seq)
                    exit(1);
            }
            _seq[_seq_used++] = (seq_t)_last_k;
        }
    }

    return _seq[(size_t)n - 1];
}
