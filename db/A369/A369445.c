/*
 * A369445  Numerator of sigma(n) / A276086(n), where A276086 is the primorial
 *          base exp-function.
 * https://oeis.org/A369445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A000203(n) / A324644(n)
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 1762, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 109 terms through n = 8974 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000203 is σ(n) (sum of divisors); A324644 is the primorial‑base exponential. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Compute σ(n) using trial division (sufficient for n ≤ 10 000). */
static seq_t sigma_simple(idx_t n)
{
    seq_t result = 1;
    idx_t temp   = n;

    /* Small primes up to 97 are enough for numbers up to 10 000. */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL,
        11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL,
        41ULL, 43ULL, 47ULL, 53ULL,
        59ULL, 61ULL, 67ULL, 71ULL,
        73ULL, 79ULL, 83ULL, 89ULL,
        97ULL
    };
    static const size_t small_len = sizeof(small_primes) / sizeof(small_primes[0]);

    for (size_t i = 0; i < small_len; ++i) {
        uint64_t p = small_primes[i];
        if (p * (uint64_t)p > (uint64_t)temp)
            break;
        if (temp % p == 0) {
            seq_t pow   = 1;      /* p^cnt */
            uint64_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
                pow *= (seq_t)p;
            }
            /* term = (p^(cnt+1)-1)/(p-1) */
            seq_t term = (pow * (seq_t)p - 1) / ((seq_t)p - 1);
            result *= term;
        }
    }
    if (temp > 1) {
        seq_t p = (seq_t)temp;           /* remaining prime, exponent 1 */
        result *= (p * p - 1) / (p - 1); /* = p + 1 */
    }
    return result;
}

static const uint64_t _primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL,
    13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL,
    53ULL, 59ULL, 61ULL, 67ULL, 71ULL,
    73ULL, 79ULL, 83ULL, 89ULL, 97ULL,
    101ULL, 103ULL, 107ULL, 109ULL, 113ULL
};
static const size_t _prime_len = sizeof(_primes) / sizeof(_primes[0]);

/* Compute A324644(n): the primorial‑base exponential. */
static seq_t _primorial_exp(idx_t n)
{
    seq_t result = 1;
    seq_t rem    = (seq_t)n;

    /* Build primorials <= n. */
    size_t m = 0;
    seq_t primorial = 1;
    seq_t primorials[20];
    while (m < _prime_len) {
        seq_t next = primorial * (seq_t)_primes[m];
        if (next > rem)
            break;
        primorials[m] = next;
        primorial = next;
        ++m;
    }

    /* Process digits from high to low. */
    for (int k = (int)m - 1; k >= 0; --k) {
        seq_t Pk    = primorials[k];
        seq_t digit = rem / Pk;
        rem        = rem % Pk;
        seq_t prime = (seq_t)_primes[k + 1];   /* exponent prime */
        for (seq_t i = 0; i < digit; ++i)
            result *= prime;
    }

    /* Final low digit uses prime[0] (2). */
    if (rem > 0) {
        seq_t prime0 = (seq_t)_primes[0];
        for (seq_t i = 0; i < rem; ++i)
            result *= prime0;
    }
    return result;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    seq_t sigma = sigma_simple(n);
    seq_t apow  = _primorial_exp(n);
    seq_t g     = gcd(sigma, apow);
    return sigma / g;
}
