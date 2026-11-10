/*
 * A331290  a(n) = gcd(A001222(n), A329348(n)).
 * https://oeis.org/A331290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = gcd(A001222(n), A329348(n)), where A001222 = bigomega and
 *            A329348 = least significant nonzero digit of the primorial base
 *            expansion of A108951(n)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9376 (2026-11-08)
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

static uint64_t *A329348_primes = NULL;
static size_t    A329348_prime_cnt = 0;
static uint64_t  A329348_limit = 0;

static void A329348_init_primes(uint64_t n)
{
    if (A329348_primes != NULL && A329348_limit >= 2 * n + 100) return;
    uint64_t limit = A329348_limit ? A329348_limit : 20000;
    while (limit < 2 * n + 100) limit *= 2;
    free(A329348_primes);
    A329348_primes = NULL;
    build_primes_upto(limit, &A329348_primes, &A329348_prime_cnt);
    if (!A329348_primes) abort();
    A329348_limit = limit;
}

static int A329348_prime_index(uint64_t p)
{
    size_t lo = 0, hi = A329348_prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (A329348_primes[mid] < p) lo = mid + 1; else hi = mid;
    }
    return (int)lo;
}

static uint64_t A329348_mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

static uint64_t A329348_val(uint64_t n)
{
    if (n == 1) return 1;

    A329348_init_primes(n);

    uint64_t pf[16], exp[16];
    size_t cnt = 0;
    factor_u64(n, pf, exp, &cnt);

    for (size_t k = 1; k <= A329348_prime_cnt; ++k) {
        uint64_t pkprime = A329348_primes[k];
        uint64_t pkmod = 1 % pkprime;
        for (size_t i = 0; i <= k - 1; ++i) {
            pkmod = (pkmod * A329348_primes[i]) % pkprime;
        }
        uint64_t inv_pkmod = A329348_mod_pow(pkmod, pkprime - 2, pkprime);

        uint64_t m_mod = 1 % pkprime;
        for (size_t i = 0; i < cnt; ++i) {
            int idx = A329348_prime_index(pf[i]);
            uint64_t prim_mod = 1 % pkprime;
            for (int j = 0; j <= idx; ++j) {
                prim_mod = (prim_mod * A329348_primes[j]) % pkprime;
            }
            m_mod = (m_mod * A329348_mod_pow(prim_mod, exp[i], pkprime)) % pkprime;
        }

        uint64_t digit = (m_mod * inv_pkmod) % pkprime;
        if (digit != 0) return digit;
    }
    return 0;
}

static uint64_t A001222_val(uint64_t n)
{
    if (n == 1) return 0;
    uint64_t count = 0;
    uint64_t temp = n;
    while (temp % 2 == 0) {
        count++;
        temp /= 2;
    }
    for (uint64_t i = 3; i <= temp / i; i += 2) {
        while (temp % i == 0) {
            count++;
            temp /= i;
        }
    }
    if (temp > 1) count++;
    return count;
}

static uint64_t A331290_gcd(uint64_t x, uint64_t y)
{
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t om = A001222_val(m);
    uint64_t d = A329348_val(m);
    return (seq_t)A331290_gcd(om, d);
}
