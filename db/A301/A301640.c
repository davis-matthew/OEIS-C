/*
 * A301640  Largest integer k such that n^2 - 3*2^k can be written as x^2 +
 *          2*y^2 with x and y integers, or -1 if no such k exists.
 * https://oeis.org/A301640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 228, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   pow2k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the largest integer k such that
 *     n^2 − 3·2^k = x^2 + 2·y^2  for some integers x, y,
 * and −1 if no such k exists.  The test for the right‑hand side
 * uses the fact that an integer m ≥ 0 is representable as x^2+2y^2
 * iff every prime ≡5 or ≡7 (mod 8) occurs in its factorisation
 * with even exponent. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2k.h"

/* -------------------------------------------------------------------- */
/* Small sieve of primes up to 10000 (sufficient for n up to ~10⁴). */

static bool primes_initialized = false;
static uint32_t primes[20000];
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes_initialized) return;
    const size_t limit = 10000;
    bool s[limit + 1];
    for (size_t i = 0; i <= limit; ++i) s[i] = true;
    s[0] = s[1] = false;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (s[i]) {
            for (size_t j = i * i; j <= limit; j += i) s[j] = false;
        }
    }
    primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (s[i]) primes[primes_cnt++] = (uint32_t)i;
    primes_initialized = true;
}

/* -------------------------------------------------------------------- */
/* Power of two in seq_t (works for small k, as needed here). */

/* -------------------------------------------------------------------- */
/* Check whether m >= 0 can be written as x^2 + 2y^2.  Uses the theorem
 * that all primes ≡5 or ≡7 (mod 8) must occur with even exponent. */

static bool representable(seq_t m)
{
    if (m < 0) return false;
    if (m == 0) return true;           /* 0 = 0^2 + 2·0^2 */

    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        if (p * p > m) break;
        unsigned int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if ((primes[i] & 7) == 5 || (primes[i] & 7) == 7) {
            if (exp & 1U) return false;
        }
    }
    if (m > 1) {  /* remaining prime factor > sqrt(original m) */
        int mod8 = (int)(m % 8);
        if (mod8 == 5 || mod8 == 7) return false;
    }
    return true;
}

/* -------------------------------------------------------------------- */
/* The required OEIS function. */

seq_t a(idx_t n)
{
    init_primes();

    if (n < 1) return (seq_t)-1;   /* not in the defined offset, return sentinel */

    seq_t n_sq = (seq_t)n * (seq_t)n;
    const seq_t three = (seq_t)3;

    /* Find the largest k with n^2 ≥ 3·2^k. */
    int max_k = -1;
    seq_t val = (seq_t)1;
    while (three * val <= n_sq) {
        val <<= 1;      /* val = 2^(max_k+1) after this shift */
        ++max_k;
    }
    if (max_k < 0) return (seq_t)-1;   /* no such k */

    for (int cur = max_k; cur >= 0; --cur) {
        seq_t pow = pow2k((idx_t)cur);
        seq_t m   = n_sq - three * pow;
        if (representable(m)) return (seq_t)cur;
    }
    return (seq_t)-1;
}
