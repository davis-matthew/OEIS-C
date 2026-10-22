/*
 * A393067  a(n) = 1 if A276086(A003415(n)) <= n, otherwise 0, where A003415
 *          is the arithmetic derivative and A276086 is the primorial base
 *          exp-function.
 * https://oeis.org/A393067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A276086(A003415(n)) <= n, else 0 * (A003415 is the
 *            arithmetic derivative; A276086 is the primorial * base
 *            exponent-function). This implementation works for any seq_t & *
 *            idx_t defined by the build and is efficient for n up to about 10
 *            000
 * Range:     u8 n <= 9865, u16 n <= 9865, u32 n <= 9865, u64 n <= 9865,
 *            u128 n <= 9865, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- primes up to 20000 ---------- */
static uint64_t _primes[2500];
static size_t _prime_cnt = 0;
/* one-time sieve to fill the prime table */
static void _init_primes(void) {
    if (_prime_cnt) return;               /* already done */
    static unsigned char sieve[20001];
    size_t i, j;
    for (i = 2; i <= 20000; ++i) sieve[i] = 0;
    for (i = 2; i * i <= 20000; ++i)
        if (!sieve[i])
            for (j = i * i; j <= 20000; j += i)
                sieve[j] = 1;
    for (i = 2; i <= 20000; ++i)
        if (!sieve[i]) _primes[_prime_cnt++] = i;
}

/* ---------- arithmetic derivative of n (uint64_t range) ---------- */
static uint64_t _arithmetic_derivative(uint64_t n) {
    if (n <= 1) return 0;
    _init_primes();
    uint64_t orig = n;
    uint64_t der = 0;
    size_t idx;
    for (idx = 0; idx < _prime_cnt && _primes[idx] * _primes[idx] <= n; ++idx) {
        uint64_t p = _primes[idx];
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) { ++exp; n /= p; }
            der += exp * (orig / p);
        }
    }
    if (n > 1) {   /* remaining prime factor */
        der += orig / n;
    }
    return der;
}

/* ---------- check A276086(m) <= limit  (using 64‑bit arithmetic) ---------- */
static bool _primorial_leq(uint64_t m, uint64_t limit) {
    if (m == 0) return limit >= 1;          /* A276086(0) = 1 */
    _init_primes();
    uint64_t prod = 1;
    uint64_t remaining = m;
    size_t idx = 0;
    while (remaining > 0 && idx < _prime_cnt) {
        uint64_t p = _primes[idx];
        uint64_t digit = remaining % p;
        remaining /= p;
        if (digit) {
            /* compute p^digit, abort if would exceed limit */
            uint64_t power = 1;
            for (uint64_t k = 0; k < digit; ++k) {
                if (power > limit / p) return false;
                power *= p;
            }
            if (power > limit / prod) return false;
            prod *= power;
        }
        ++idx;
    }
    /* remaining should be 0 for m <= 20000; otherwise product already
       exceeded limit or will be > limit. */
    return prod <= limit;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t der = _arithmetic_derivative(N);
    bool ok   = _primorial_leq(der, N);
    return ok ? (seq_t)1 : (seq_t)0;
}
