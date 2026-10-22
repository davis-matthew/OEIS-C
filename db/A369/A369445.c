/*
 * A369445  Numerator of sigma(n) / A276086(n), where A276086 is the primorial
 *          base exp-function.
 * https://oeis.org/A369445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A000203(n) / A324644(n)
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 1762, u64 n <= 8974,
 *            u128 n <= 8974, big any n
 * Helpers:   gcd, sigma_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000203 is σ(n) (sum of divisors); A324644 is the primorial‑base exponential. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/sigma_simple.h"

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
