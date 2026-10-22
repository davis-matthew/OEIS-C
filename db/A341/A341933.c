/*
 * A341933  a(n) = A023896(n) mod A000203(n).
 * https://oeis.org/A341933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A023896(n) mod A000203(n)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A023896(n) = sum of totatives of n */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* ---- Prime sieve (up to max n) ---- */
#define MAX_PRIME 10000
static uint64_t _primes[MAX_PRIME];
static size_t _prime_cnt = 0;
static bool _primes_ready = false;

static void _build_primes(void) {
    if (_prime_cnt) return;
    static bool sieve[MAX_PRIME + 1];
    memset(sieve, true, sizeof(sieve));
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= MAX_PRIME; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= MAX_PRIME; j += i) {
                sieve[j] = false;
            }
        }
    }
    for (size_t i = 2; i <= MAX_PRIME; ++i) {
        if (sieve[i]) _primes[_prime_cnt++] = i;
    }
    _primes_ready = true;
}

/* ---- Euler's totient phi(n) ---- */
static uint64_t _phi_uint64(uint64_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;          /* φ(1) = 1 by convention */
    _build_primes();
    uint64_t result = n;
    uint64_t m = n;
    for (size_t i = 0; i < _prime_cnt && _primes[i] * _primes[i] <= m; ++i) {
        uint64_t p = _primes[i];
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

/* ---- Sum of divisors σ(n) ---- */
static uint64_t _sigma_uint64(uint64_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;          /* σ(1) = 1 */
    _build_primes();
    uint64_t result = 1;
    uint64_t m = n;
    for (size_t i = 0; i < _prime_cnt && _primes[i] * _primes[i] <= m; ++i) {
        uint64_t p = _primes[i];
        if (m % p == 0) {
            uint64_t term = 1;
            uint64_t power = 1;
            while (m % p == 0) {
                m /= p;
                power *= p;
                term += power;
            }
            result *= term;
        }
    }
    if (m > 1) { /* remaining prime factor */
        result *= (1 + m);
    }
    return result;
}

/* ---- Main sequence function ---- */
seq_t a(idx_t n) {
    /* n is at least 1 by the OEIS offset */
    uint64_t un = (uint64_t)n;
    /* sum of totatives: n*phi(n)/2, except n=1 -> 0 */
    uint64_t tot_sum;
    if (un <= 1) {
        tot_sum = 0;
    } else {
        uint64_t phi_val = _phi_uint64(un);
        __int128 prod = (__int128)un * (__int128)phi_val;
        tot_sum = (uint64_t)(prod / 2);
    }
    uint64_t sigma_val = _sigma_uint64(un);
    uint64_t result = sigma_val ? (tot_sum % sigma_val) : 0;
    return (seq_t)result;
}
