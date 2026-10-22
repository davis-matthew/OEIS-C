/*
 * A359778  Number of factorizations of n into factors not divisible by p^p
 *          for any prime p (terms of A048103).
 * https://oeis.org/A359778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of multiplicative partitions of n into factors not
 *            divisible by p^p for any prime p
 * Range:     u8 n <= 8827, u16 n <= 8827, u32 n <= 8827, u64 n <= 8827,
 *            u128 n <= 8827, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper constants and tables */
static bool _init_done = false;
static idx_t  _limit = 0;
static idx_t  * _spf   = NULL;   /* smallest prime factor */
static bool   * _allowed = NULL; /* factor allowed? */
static seq_t  * _cache  = NULL;  /* memoised a(n) values */

/* Grow tables to at least new_limit */
static void _grow_tables(idx_t new_limit)
{
    _spf      = (idx_t  *)realloc(_spf,      (new_limit + 1) * sizeof(idx_t));
    _allowed  = (bool   *)realloc(_allowed,  (new_limit + 1) * sizeof(bool));
    _cache    = (seq_t  *)realloc(_cache,    (new_limit + 1) * sizeof(seq_t));

    for (idx_t i = _limit + 1; i <= new_limit; ++i) {
        _spf[i] = 0;
        _allowed[i] = false;
        _cache[i] = 0;
    }
    _limit = new_limit;
}

/* Initialise SPF, allowed table and cache up to at least n */
static void _init_tables(idx_t n)
{
    if (n <= _limit) return;

    /* grow to next power of two >= n */
    idx_t new_limit = _limit ? _limit : 256;
    while (new_limit < n) new_limit <<= 1;
    _grow_tables(new_limit);

    /* compute smallest prime factors */
    for (idx_t i = 2; i <= new_limit; ++i) _spf[i] = i;
    idx_t bound = 1;
    while (bound * bound <= new_limit) ++bound; /* bound = floor(sqrt(new_limit)) + 1 */
    --bound;
    for (idx_t i = 2; i <= bound; ++i) {
        if (_spf[i] == i) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                if (_spf[j] == j) _spf[j] = i;
        }
    }

    /* compute allowedness for every number */
    _allowed[1] = true;
    for (idx_t i = 2; i <= new_limit; ++i) {
        seq_t ok = 1;
        idx_t x = i;
        while (x > 1) {
            idx_t p = _spf[x];
            seq_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            if (e >= p) { ok = 0; break; }
        }
        _allowed[i] = (bool)ok;
    }

    /* initialise cache */
    _cache[1] = 1;        /* a(1)=1 */
}

static seq_t _count_part(idx_t rem, idx_t min_factor)
{
    seq_t total = 0;
    for (idx_t d = min_factor; d <= rem; ++d) {
        if (rem % d == 0 && _allowed[d]) {
            idx_t rest = rem / d;
            if (rest == 1)
                ++total;
            else
                total += _count_part(rest, d);
        }
    }
    return total;
}

static seq_t _compute_a(idx_t n)
{
    if (n == 1) return 1;

    seq_t result = 0;
    for (idx_t d = 2; d <= n; ++d) {
        if (n % d == 0 && _allowed[d]) {
            idx_t rest = n / d;
            if (rest == 1)
                ++result;
            else
                result += _count_part(rest, d);
        }
    }
    return result;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined, defensive */
    _init_tables(n);

    seq_t cached = _cache[n];
    if (cached != 0 || n == 1)  /* cache[1] already known */
        return cached;

    seq_t val = _compute_a(n);
    _cache[n] = val;
    return val;
}
