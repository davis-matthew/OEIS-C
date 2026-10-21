/*
 * A247176  Largest number of maximal order mod n.
 * https://oeis.org/A247176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 258, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: largest integer a < n whose multiplicative order
   modulo n equals the Carmichael function λ(n) (maximum possible order).
   Returns 0 for n = 1. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*--------------------------------------------------------------------
 * Prime table up to 10000 (sufficient for n ≤ 10000)
 *--------------------------------------------------------------------*/
#define PRIME_LIMIT 10000
static bool _prime_sieve[PRIME_LIMIT + 1];
static unsigned int _primes[1229];   /* π(10000)=1229 */
static size_t _prime_cnt = 0;
static void _init_primes(void)
{
    if (_prime_cnt > 0) return;          /* already built */
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        _prime_sieve[i] = true;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (_prime_sieve[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                _prime_sieve[j] = false;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (_prime_sieve[i])
            _primes[_prime_cnt++] = (unsigned int)i;
}

/*--------------------------------------------------------------------
 * Basic arithmetic helpers (work on 64‑bit unsigned values)
 *--------------------------------------------------------------------*/
static uint64_t _gcd_uint64(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}
static uint64_t _lcm_uint64(uint64_t a, uint64_t b)
{
    return (a / _gcd_uint64(a, b)) * b;
}
static uint64_t _mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}
static uint64_t _mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    uint64_t cur = base % mod;
    while (exp) {
        if (exp & 1) res = _mod_mul(res, cur, mod);
        cur = _mod_mul(cur, cur, mod);
        exp >>= 1;
    }
    return res;
}

/*--------------------------------------------------------------------
 * Carmichael function λ(n)  (n ≥ 1)
 *--------------------------------------------------------------------*/
static uint64_t _lambda_of(uint64_t n)
{
    if (n == 1) return 1;            /* convenient sentinel */
    uint64_t result = 1;
    uint64_t tmp = n;
    for (size_t i = 0; i < _prime_cnt && (uint64_t)_primes[i] * _primes[i] <= tmp; ++i) {
        unsigned int p = _primes[i];
        if (tmp % p) continue;
        int e = 0;
        while (tmp % p == 0) { tmp /= p; ++e; }
        uint64_t lambda_p;
        if (p == 2) {
            if (e == 1)            lambda_p = 1;
            else if (e == 2)       lambda_p = 2;
            else                   lambda_p = ((uint64_t)1) << (e - 2);
        } else {
            lambda_p = (uint64_t)(p - 1);
            for (int k = 1; k < e; ++k) lambda_p *= p;
        }
        result = _lcm_uint64(result, lambda_p);
    }
    if (tmp > 1) {                    /* remaining prime factor */
        uint64_t lambda_p = tmp - 1;
        result = _lcm_uint64(result, lambda_p);
    }
    return result;
}

/*--------------------------------------------------------------------
 * Distinct prime factors of a number (store in array)
 *--------------------------------------------------------------------*/
static void _factor_distinct(uint64_t n, uint64_t *factors, size_t *cnt)
{
    *cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < _prime_cnt && (uint64_t)_primes[i] * _primes[i] <= tmp; ++i) {
        unsigned int p = _primes[i];
        if (tmp % p == 0) {
            factors[(*cnt)++] = (uint64_t)p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) factors[(*cnt)++] = tmp;
}

/*--------------------------------------------------------------------
 * Main sequence function
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    _init_primes();

    if (n <= 1) return (seq_t)0;   /* a(1) = 0 by definition */

    uint64_t lam = _lambda_of((uint64_t)n);
    /* factor λ(n) into distinct primes */
    uint64_t lam_factors[64];
    size_t lam_cnt;
    _factor_distinct(lam, lam_factors, &lam_cnt);

    /* search downward from n-1 */
    for (idx_t x = n - 1; x >= 1; --x) {
        if (_gcd_uint64((uint64_t)x, (uint64_t)n) != 1) continue; /* not coprime */

        uint64_t ord = lam;
        for (size_t i = 0; i < lam_cnt; ++i) {
            uint64_t p = lam_factors[i];
            while (ord % p == 0) {
                uint64_t cand = ord / p;
                if (_mod_pow_uint64((uint64_t)x, cand, (uint64_t)n) == 1)
                    ord = cand;
                else
                    break;
            }
        }
        if (ord == lam) return (seq_t)x;   /* maximal order achieved */
    }

    return (seq_t)0;    /* should never be reached */
}
