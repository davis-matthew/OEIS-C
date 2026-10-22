/*
 * A326810  The smallest prime that does not divide the prime product form
 *          (A276086) of the primorial base expansion of n.
 * https://oeis.org/A326810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime that does not divide the prime product
 *            form of n in primorial base (A276086)
 * Range:     u8 n <= 9762, u16 n <= 9762, u32 n <= 9762, u64 n <= 9762,
 *            u128 n <= 9762, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define MAX_PRIMES 128

static uint32_t _primes[MAX_PRIMES];
static size_t _primeCnt = 0;
static unsigned char _used[MAX_PRIMES];

/* Ensure that at least 'upto' primes are stored in _primes. */
static void _ensure_primes(size_t upto)
{
    if (_primeCnt >= upto)
        return;
    if (_primeCnt == 0) {
        _primes[0] = 2;
        _primeCnt = 1;
    }
    uint32_t candidate = _primes[_primeCnt - 1] + 1;
    while (_primeCnt < upto) {
        int isPrime = 1;
        for (size_t j = 0; j < _primeCnt; ++j) {
            uint64_t sq = (uint64_t)_primes[j] * _primes[j];
            if (sq > candidate)
                break;
            if (candidate % _primes[j] == 0) {
                isPrime = 0;
                break;
            }
        }
        if (isPrime) {
            if (_primeCnt < MAX_PRIMES)
                _primes[_primeCnt++] = candidate;
            else
                return; /* safety, but not expected for n up to 10000 */
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    /* Clear used markers. */
    memset(_used, 0, sizeof _used);

    idx_t rem = n;
    size_t i = 0;

    /* Compute primorial base digits; record used primes. */
    while (rem > 0) {
        _ensure_primes(i + 1);
        uint32_t p = _primes[i];
        idx_t digit = rem % p;
        if (digit > 0)
            _used[i] = 1;
        rem /= p;
        ++i;
    }

    /* Find the smallest prime not used. */
    for (size_t j = 0; j < _primeCnt; ++j) {
        if (_used[j] == 0)
            return (seq_t)_primes[j];
    }

    /* All existing primes were used; need the next one. */
    size_t newIdx = _primeCnt;
    _ensure_primes(newIdx + 1);
    return (seq_t)_primes[newIdx];
}
