/*
 * A328237  Quotient A003415(n+k)/A003415(n) for the least k > 0 for which
 *          such a quotient is an integer. Here A003415(x) is the arithmetic
 *          derivative of x.
 * https://oeis.org/A328237
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 9720, u32 n <= 9720, u64 n <= 9720,
 *            u128 n <= 9720, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the least k>0 with A003415(n+k)/A003415(n) an integer
   and return that quotient.  A003415 is the arithmetic derivative. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- Smallest prime factor sieve (shared across calls) ---------- */
static idx_t * _spf = NULL;           /* smallest prime factor for each n */
static size_t _spf_size = 0;

/* Build a sieve of smallest prime factors up to a chosen limit.
   The limit is large enough to handle all n ≤ 10000 and the
   subsequent search range.  The function is idempotent. */
static void _init_spf(void)
{
    static bool _initialized = false;
    if (_initialized) return;

    const idx_t LIMIT = 200000;          /* > 10000 + expected k search range */
    _spf = (idx_t *)malloc(sizeof(idx_t) * (LIMIT + 1));
    if (!_spf) exit(1);                   /* abort on OOM */
    memset(_spf, 0, sizeof(idx_t) * (LIMIT + 1));

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (_spf[i] == 0) {               /* i is prime */
            _spf[i] = i;
            if (i * i <= LIMIT) {
                for (idx_t j = i * i; j <= LIMIT; j += i) {
                    if (_spf[j] == 0) _spf[j] = i;
                }
            }
        }
    }
    _spf_size = LIMIT + 1;
    _initialized = true;
}

/* Compute the arithmetic derivative of n (n ≥ 0).  For n ≤ 1 it is 0. */
static seq_t _arith_derivative(idx_t n)
{
    if (n <= 1) return 0;
    seq_t d = 0;
    idx_t orig = n;
    while (n > 1) {
        idx_t p = (_spf && n < _spf_size) ? _spf[n] : 0;
        if (p == 0) p = n;                /* n is prime larger than sieve */
        idx_t e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        d += (seq_t)e * (seq_t)(orig / p);
    }
    return d;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                  /* undefined for offset < 2 */

    _init_spf();                           /* ensure sieve prepared */

    seq_t d_n = _arith_derivative(n);
    if (d_n == 0) return 0;                /* safety, should not happen */

    for (idx_t k = 1; ; ++k) {
        seq_t d_nk = _arith_derivative(n + k);
        if (d_nk % d_n == 0) {
            return d_nk / d_n;
        }
    }
}
