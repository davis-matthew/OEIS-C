/*
 * A260187  a(n) = n modulo the greatest primorial <= n.
 * https://oeis.org/A260187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n mod (greatest primorial <= n)
 * Range:     u8 n <= 2565, u16 n <= 9262, u32 n <= 9262, u64 n <= 9262,
 *            u128 n <= 9262, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* List of primes used to form primorials */
static const uint64_t _primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL,
    29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL,
    61ULL, 67ULL, 71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL,
    101ULL, 103ULL, 107ULL, 109ULL, 113ULL
};
static const size_t _primes_len = sizeof(_primes) / sizeof(_primes[0]);

/* Lazy‑initialized array of primorials */
static uint64_t * _primorials = NULL;
static size_t     _primorial_cnt = 0;
static int        _primorials_inited = 0;

/* Build primorials on first use */
static void _init_primorials(void)
{
    if (_primorials_inited)
        return;
    if (_primes_len == 0)
        return;

    _primorials = (uint64_t *)malloc(_primes_len * sizeof(uint64_t));
    if (_primorials == NULL)
        return;  /* Out of memory; the function will just return 0 */

    _primorials[0] = _primes[0];
    _primorial_cnt = 1;
    for (size_t i = 1; i < _primes_len; ++i) {
        _primorials[i] = _primorials[i - 1] * _primes[i];
        ++_primorial_cnt;
    }
    _primorials_inited = 1;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* a(1) = 0. For n = 2 the proper primorial is 2#, giving 0 as well. */
    if (n < 2)
        return (seq_t)0;

    _init_primorials();
    if (!_primorials_inited)
        return (seq_t)0;  /* Fallback if memory allocation failed */

    /* Find the greatest primorial <= n */
    size_t idx = 0;
    while (idx + 1 < _primorial_cnt && _primorials[idx + 1] <= (uint64_t)n)
        ++idx;

    uint64_t p   = _primorials[idx];
    uint64_t res = ((uint64_t)n) % p;
    return (seq_t)res;
}
