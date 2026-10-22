/*
 * A327405  Quotient of n over the maximum divisor of n that is 1 or whose
 *          prime indices have a common divisor > 1.
 * https://oeis.org/A327405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n / d_max * where d_max is the largest divisor of n that
 *            is 1 or whose prime * indices have a common divisor > 1. The
 *            function enumerates all * divisors of n, checks the gcd of the
 *            prime indices of each divisor, * keeps the maximum that
 *            satisfies the condition, and returns n divided * by t...
 * Range:     u8 n <= 255, u16 n <= 9894, u32 n <= 9894, u64 n <= 9894,
 *            u128 n <= 9894, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/*--- static workspace --------------------------------------------------*/
static idx_t  _limit_prepared = 0;   /* largest n for which tables are ready */
static uint32_t * _spf          = NULL;          /* smallest prime factor */
static uint32_t * _prime_index  = NULL;          /* 1‑based index of prime */
static size_t    _prime_cnt    = 0;              /* number of primes found */

/*--- prepare prime tables up to max_n ---------------------------------*/
static void _prepare_primes(idx_t max_n)
{
    if (max_n <= _limit_prepared) return;

    /* resize arrays */
    if (_spf == NULL) {
        _spf       = (uint32_t*)malloc((max_n + 1) * sizeof(uint32_t));
        _prime_index = (uint32_t*)malloc((max_n + 1) * sizeof(uint32_t));
    } else {
        _spf       = (uint32_t*)realloc(_spf,       (max_n + 1) * sizeof(uint32_t));
        _prime_index = (uint32_t*)realloc(_prime_index, (max_n + 1) * sizeof(uint32_t));
    }
    /* initialize newly added region */
    for (idx_t i = _limit_prepared + 1; i <= max_n; ++i) {
        _spf[i] = 0;
        _prime_index[i] = 0;
    }

    /* sieve for smallest prime factor and prime indices */
    _prime_cnt = 0;
    for (idx_t i = 2; i <= max_n; ++i) _spf[i] = 0;
    for (idx_t i = 2; i <= max_n; ++i) {
        if (_spf[i] == 0) {
            _spf[i] = (uint32_t)i;
            _prime_index[i] = ++_prime_cnt;
            if ((uint64_t)i * (uint64_t)i <= (uint64_t)max_n) {
                for (uint64_t j = (uint64_t)i * (uint64_t)i; j <= (uint64_t)max_n; j += (uint64_t)i) {
                    if (_spf[j] == 0) _spf[j] = (uint32_t)i;
                }
            }
        }
    }
    _limit_prepared = max_n;
}

/*--- recursive divisor enumeration -------------------------------------*/
static void _dfs_divisors(int pos, int k,
                          const uint64_t *primes, const int *exps,
                          seq_t val, seq_t cur_gcd,
                          seq_t *max_div)
{
    if (pos == k) {
        /* divisor is val; allowed if val==1 or cur_gcd>1 */
        if (val == (seq_t)1 || cur_gcd > (seq_t)1) {
            if (val > *max_div) *max_div = val;
        }
        return;
    }

    seq_t val_mult = 1;
    for (int e = 0; e <= exps[pos]; ++e) {
        seq_t new_val = val * val_mult;
        seq_t new_gcd = cur_gcd;
        if (e > 0) {
            uint32_t idxp = _prime_index[(size_t)primes[pos]];
            new_gcd = (new_gcd == 0) ? (seq_t)idxp : gcd(new_gcd, (seq_t)idxp);
        }
        _dfs_divisors(pos + 1, k, primes, exps, new_val, new_gcd, max_div);
        val_mult *= (seq_t)primes[pos];
    }
}

/*--- public entry point -------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)n;        /* a(1) = 1 */

    /* ensure tables for this n */
    _prepare_primes(n);

    /* factorise n */
    idx_t tmp = n;
    /* allocate temporary arrays for factors */
    uint64_t *pr = NULL;
    int *exp = NULL;
    size_t k = 0;
    do {
        uint32_t p = _spf[tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        ++k;
        pr   = (uint64_t*)realloc(pr,   k * sizeof(uint64_t));
        exp  = (int*)     realloc(exp,  k * sizeof(int));
        pr[k-1]  = (uint64_t)p;
        exp[k-1] = e;
    } while (tmp > 1);

    /* enumerate all divisors and keep the maximum allowed one */
    seq_t max_div = 1;    /* divisor 1 is always allowed */
    _dfs_divisors(0, (int)k, pr, exp, (seq_t)1, (seq_t)0, &max_div);

    free(pr);
    free(exp);

    /* compute a(n) = n / max_div */
    return (seq_t)n / max_div;
}
