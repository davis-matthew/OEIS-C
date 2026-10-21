/*
 * A243283  One more than the partial sums of the characteristic function of
 *          A070003.
 * https://oeis.org/A243283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5042, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = count_{m ≤ n} [ m is divisible by the square of its
   largest prime divisor ] */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build system. */
static idx_t  _max_n = 0;
static seq_t * _pref  = NULL;   /* prefix sums of qualifying numbers   */
static idx_t * _spf   = NULL;   /* smallest prime factor for each n   */
static idx_t * _primes= NULL;   /* list of primes for the sieve       */
static size_t  _prime_count = 0;

/* Recompute the sieve and prefix array up to at least n. */
static void
ensure_precomp(idx_t n)
{
    if (n <= _max_n) return;
    if (n == 0) return;

    /* Allocate or grow arrays. */
    _spf   = (idx_t *)realloc(_spf,   (size_t)(n + 1) * sizeof(idx_t));
    _pref  = (seq_t *)realloc(_pref,  (size_t)(n + 1) * sizeof(seq_t));
    _primes= (idx_t *)realloc(_primes, (size_t)(n + 1) * sizeof(idx_t));
    if (!_spf || !_pref || !_primes) exit(EXIT_FAILURE);

    /* Initialize spf array to zero. */
    for (size_t i = 0; i <= (size_t)n; ++i) _spf[i] = 0;
    _spf[0] = 0; _spf[1] = 1;

    /* Linear sieve to fill spf and primes. */
    _prime_count = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (_spf[i] == 0) {
            _spf[i] = i;
            _primes[_prime_count++] = i;
        }
        for (size_t j = 0; j < _prime_count && _primes[j] <= _spf[i]; ++j) {
            idx_t p = _primes[j];
            if (i > n / p) break;
            _spf[i * p] = p;
        }
    }

    /* Build prefix sums of qualifying numbers. */
    _pref[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        bool qualify = false;
        if (i == 1) {
            qualify = true;
        } else {
            idx_t m = i;
            idx_t largest_p  = 0;
            idx_t largest_exp = 0;
            while (m > 1) {
                idx_t p = _spf[m];
                idx_t exp = 0;
                while (m % p == 0) { m /= p; ++exp; }
                if (p > largest_p) {
                    largest_p   = p;
                    largest_exp = exp;
                }
            }
            qualify = (largest_exp >= 2);
        }
        _pref[i] = _pref[i - 1] + (qualify ? 1 : 0);
    }

    _max_n = n;
}

/* Public sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;   /* behavior for n<1 is irrelevant */
    ensure_precomp(n);
    return _pref[n];
}
