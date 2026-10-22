/*
 * A350959  Number of ways to write 2*n+1 as 2^i (i >= 0) plus a prime.
 * https://oeis.org/A350959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the distinct pairs (i,p) with i >= 0, p prime, such
 *            that 2*n+1 = 2^i + p. That is, the number of ways to write an
 *            odd integer 2*n+1 as a sum of a power of two and a prime
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation note: For n <= 10000, 2*n+1 <= 20001.  The exponent i satisfies
   2^i <= 2*n+1, so i <= 15.  We simply test each i and use a simple sieve for
   primes up to 20001.  The sieve is constructed once on the first call.
*/

#include <stdbool.h>
#include <stdint.h>

#ifndef MAX_PRIME
#define MAX_PRIME 20001
#endif

static bool _a_inited = false;
static uint8_t _a_isprime[MAX_PRIME + 1];

static void _build_sieve(void)
{
    /* Initialize all as prime */
    for (idx_t i = 0; i <= MAX_PRIME; ++i)
        _a_isprime[i] = 1;
    /* 0 and 1 are not prime */
    if (MAX_PRIME >= 0) _a_isprime[0] = 0;
    if (MAX_PRIME >= 1) _a_isprime[1] = 0;

    for (idx_t p = 2; p * p <= MAX_PRIME; ++p) {
        if (_a_isprime[p]) {
            for (idx_t q = p * p; q <= MAX_PRIME; q += p)
                _a_isprime[q] = 0;
        }
    }
}

seq_t a(idx_t n)
{
    if (!_a_inited) {
        _build_sieve();
        _a_inited = true;
    }

    idx_t value = 2 * n + 1;      /* the odd integer to represent */
    seq_t count = 0;

    /* For each i with 2^i <= value */
    for (idx_t i = 0; (1ULL << i) <= value; ++i) {
        idx_t candidate = value - (1ULL << i);
        if (candidate >= 2 && candidate <= MAX_PRIME) {
            if (_a_isprime[candidate])
                ++count;
        }
    }

    return count;
}
