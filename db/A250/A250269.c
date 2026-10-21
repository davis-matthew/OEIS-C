/*
 * A250269  Primitive part of n! (for n>=1): n! = Product_{d|n} a(d).
 * https://oeis.org/A250269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{d|n} (d!)^{μ(n/d)} (μ = Möbius function)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   _divisors, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* External helpers provided by the build system */
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"
#include "../utils/_divisors.h"

static size_t  _spf_size = 0;          /* current upper bound of SPF array */
static uint32_t *_spf = NULL;          /* smallest prime factor for each number */
static size_t   _prime_count = 0;      /* number of primes discovered so far */
static uint64_t *_primes = NULL;       /* list of primes up to _spf_size */

/* Build SPF and prime list up to max_n (inclusive) */
static void _build_spf_primes(idx_t max_n)
{
    if (max_n <= _spf_size)
        return;                       /* already built */

    /* free old arrays */
    free(_spf);
    free(_primes);

    /* allocate SPF array */
    _spf = (uint32_t *)calloc((size_t)max_n + 1, sizeof(uint32_t));
    if (!_spf) exit(EXIT_FAILURE);

    /* Sieve for smallest prime factor */
    for (idx_t i = 2; i <= max_n; ++i)
        _spf[i] = (uint32_t)i;

    for (idx_t i = 2; i * i <= max_n; ++i)
        if (_spf[i] == (uint32_t)i) {         /* i is prime */
            for (idx_t j = i * i; j <= max_n; j += i)
                if (_spf[j] == (uint32_t)j)
                    _spf[j] = (uint32_t)i;
        }

    /* Build prime list */
    _prime_count = 0;
    for (idx_t i = 2; i <= max_n; ++i)
        if (_spf[i] == (uint32_t)i)
            ++_prime_count;

    _primes = (uint64_t *)malloc(_prime_count * sizeof(uint64_t));
    if (!_primes) exit(EXIT_FAILURE);

    idx_t idx = 0;
    for (idx_t i = 2; i <= max_n; ++i)
        if (_spf[i] == (uint32_t)i)
            _primes[idx++] = i;

    _spf_size = max_n;
}

/* Möbius function using precomputed SPF */
static int _mobius(idx_t x)
{
    if (x == 1) return 1;
    int mu = 1;
    while (x > 1) {
        uint32_t p = _spf[x];
        int cnt = 0;
        while (x % p == 0) {
            x /= p;
            cnt++;
            if (cnt > 1)
                return 0;                /* not square‑free */
        }
        mu = -mu;
    }
    return mu;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;            /* undefined for n=0, return 1 */

    /* Ensure SPF and prime list cover n */
    _build_spf_primes(n);

    /* Temporary array for prime exponents */
    static int prime_exp[2000];
    memset(prime_exp, 0, _prime_count * sizeof(int));

    /* Find all divisors of n */
    idx_t divs[256];
    idx_t div_cnt = _divisors(n, divs, 256);

    /* Process each divisor d */
    for (idx_t di = 0; di < div_cnt; ++di) {
        idx_t d = divs[di];
        int mu = _mobius(n / d);
        if (mu == 0) continue;              /* skip if μ = 0 */

        /* Add exponent of each prime in d! according to μ */
        for (size_t pi = 0; pi < _prime_count; ++pi) {
            uint64_t p = _primes[pi];
            if (p > d) break;
            int e = 0;
            for (idx_t div = p; div <= d; div *= p)
                e += d / div;
            prime_exp[pi] += mu * e;
        }
    }

    /* Build the result from prime exponents (all should be ≥0) */
    seq_t result = (seq_t)1;
    for (size_t pi = 0; pi < _prime_count; ++pi) {
        int e = prime_exp[pi];
        if (e == 0) continue;
        seq_t pwr = pow_uint(_primes[pi], (idx_t)e);
        result = seq_mul(result, pwr);
    }
    return result;
}
