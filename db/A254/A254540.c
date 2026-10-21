/*
 * A254540  Number of length 2 1..(n+2) arrays with no leading partial sum
 *          equal to a prime.
 * https://oeis.org/A254540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration of length‑2 arrays over [1..n+2] with no prime prefix sums:
   a(n) = Σ_{x=1}^{n+2, x not prime} ((n+2) - (π(x+n+2)-π(x)))   */
/* The function maintains a static prime sieve up to the required limit
   so subsequent calls with larger n reuse the same data.   */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t   _maxS = 0;          /* maximum S = n+2 used so far      */
static uint8_t *_is_prime = NULL;  /* prime flag array up to limit     */
static idx_t  *_prime_prefix = NULL;/* prefix counts of primes          */
static size_t  _cap = 0;           /* current capacity of the arrays   */

/* Ensure a sieve and prefix array exist for all numbers up to `limit`. */
static void _ensure_sieve(size_t limit)
{
    if (limit <= _cap)
        return;                          /* no need to rebuild          */

    /* free old data if any */
    free(_is_prime);
    free(_prime_prefix);

    _cap = limit;
    _is_prime = (uint8_t *)malloc(_cap + 1);
    _prime_prefix = (idx_t *)malloc((_cap + 1) * sizeof(idx_t));

    /* initialise with all true   */
    for (size_t i = 0; i <= _cap; ++i)
        _is_prime[i] = 1;

    /* 0 and 1 are not prime   */
    if (_cap >= 0) _is_prime[0] = 0;
    if (_cap >= 1) _is_prime[1] = 0;

    /* Sieve of Eratosthenes   */
    for (size_t p = 2; p * p <= _cap; ++p) {
        if (_is_prime[p]) {
            for (size_t j = p * p; j <= _cap; j += p)
                _is_prime[j] = 0;
        }
    }

    /* Build prefix sums of prime counts   */
    _prime_prefix[0] = 0;
    for (size_t i = 1; i <= _cap; ++i)
        _prime_prefix[i] = _prime_prefix[i - 1] + (idx_t)_is_prime[i];
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* value below offset is unused */

    /* Convert index to problem size.  S = n+2   */
    idx_t S = n + 2;
    size_t limit = 2 * (size_t)S;   /* highest possible sum */

    /* Extend sieve if necessary   */
    if (S > _maxS) {
        _ensure_sieve(limit);
        _maxS = S;
    }

    seq_t ans = 0;

    for (idx_t x = 1; x <= S; ++x) {
        if (_is_prime[(size_t)x])      /* x must not be prime   */
            continue;
        /* count primes in (x, x+S]   */
        idx_t primes_in_range = _prime_prefix[x + S] - _prime_prefix[x];
        ans += (seq_t)(S - primes_in_range);
    }

    return ans;
}
