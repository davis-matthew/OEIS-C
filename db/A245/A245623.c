/*
 * A245623  Primes having more digits 3's than 1's.
 * https://oeis.org/A245623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime that contains more digit 3's than 1's
 * Range:     u8 n <= 10, u16 n <= 2161, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, digit_counts, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/digit_counts.h"
#include "../utils/append_candidate.h"

static uint64_t _next_candidate = 2;      /* number to test for primeness */
static seq_t * _qual_primes = NULL;       /* array of qualifying primes */
static size_t  _qual_len = 0;             /* current number of stored primes */
static size_t  _qual_cap = 0;             /* capacity of the array */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n < 1, return 0 */

    /* Keep searching until we have at least n qualifying primes */
    while (_qual_len < n) {
        uint64_t p = _next_candidate;
        if (p <= 2) p = 3;       /* skip 2 (it has 0 3's and 0 1's) */

        /* Find the next prime */
        while (!is_prime(p)) ++p;

        /* Count digits of the prime */
        unsigned char cnt[10] = {0};
        digit_counts((seq_t)p, cnt);

        /* If it has more 3's than 1's, store it */
        if (cnt[3] > cnt[1]) {
            append_candidate(&_qual_primes, &_qual_len, &_qual_cap, (seq_t)p);
        }

        _next_candidate = p + 1;   /* next trial starts after this prime */
    }

    return _qual_primes[n - 1];
}
