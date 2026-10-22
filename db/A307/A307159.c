/*
 * A307159  Partial sums of the bi-unitary divisors sum function: Sum_{k=1..n}
 *          bsigma(k), where bsigma is A188999.
 * https://oeis.org/A307159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 294, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS: A307159
   Formula: a(n) = Σ_{k=1..n} bsigma(k), where bsigma is the sum of bi‑unitary divisors 
   (A188999).  bsigma is multiplicative and
       bsigma(p^e) = σ(p^e)                     if e is odd
       bsigma(p^e) = σ(p^e) – p^(e/2)           if e is even
   where σ(p^e) = 1 + p + … + p^e.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static caches for computed values */
static idx_t _max_n = 0;
static uint64_t * _spf  = NULL;     /* smallest prime factor */
static seq_t   * _prefix = NULL;    /* prefix sums a(n) */

/* Compute smallest prime factor table up to N */
static void _sieve_spf(idx_t N)
{
    uint64_t *spf = (uint64_t*)calloc((size_t)(N + 1), sizeof(uint64_t));
    if (!spf) return;               /* allocation failure */
    for (idx_t i = 2; i <= N; ++i)
        if (!spf[i]) {
            spf[i] = i;
            if (i <= N / i)
                for (idx_t j = i * i; j <= N; j += i)
                    if (!spf[j]) spf[j] = i;
        }
    free(_spf);
    _spf = spf;
}

/* Compute σ(p^e) as a sum of powers of p */
static seq_t _sigma_prime_power(uint64_t p, int e)
{
    seq_t sum = 1;          /* start with p^0 */
    seq_t term = 1;
    for (int i = 1; i <= e; ++i) {
        term *= (seq_t)p;
        sum   += term;
    }
    return sum;
}

/* Compute bsigma(n) using the multiplicative formula and _spf table */
static seq_t _bsigma_n(idx_t n)
{
    if (n == 1) return (seq_t)1;
    seq_t result = 1;
    idx_t tmp   = n;
    while (tmp > 1) {
        uint64_t p  = _spf[tmp];
        int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        seq_t sigma = _sigma_prime_power(p, exp);
        if ((exp & 1) == 0) {             /* even exponent */
            seq_t half_pow = 1;
            for (int i = 0; i < exp / 2; ++i)
                half_pow *= (seq_t)p;
            sigma -= half_pow;
        }
        result *= sigma;
    }
    return result;
}

/* Extend computation up to N, filling _prefix array */
static void _compute_up_to(idx_t N)
{
    if (N <= _max_n) return;
    /* Reallocate prefix array */
    seq_t *new_prefix = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!new_prefix) return;   /* allocation failure */
    _sieve_spf(N);
    new_prefix[0] = 0;
    new_prefix[1] = 1;
    for (idx_t i = 2; i <= N; ++i) {
        seq_t bs = _bsigma_n(i);
        new_prefix[i] = new_prefix[i - 1] + bs;
    }
    free(_prefix);
    _prefix = new_prefix;
    _max_n = N;
}

/* Public function required by the specification */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    _compute_up_to(n);
    return _prefix[n];
}
