/*
 * A353374  a(n) = 1 if the prime factorization of n has an even number of
 *          prime factors that sum to an even number, otherwise 0.
 * https://oeis.org/A353374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if prime factorization of n has an even number of prime
 *            factors and the sum of those prime factors is even; otherwise 0
 * Range:     u8 n <= 9566, u16 n <= 9566, u32 n <= 9566, u64 n <= 9566,
 *            u128 n <= 9566, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies: build_spf */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

/* Static cache for smallest prime factors */
static idx_t _spf_limit = 0;
static idx_t * _spf = NULL;

/* Helper to ensure SPF cache up to n */
static void _ensure_spf(idx_t n)
{
    if (n <= _spf_limit) return;
    /* Free old cache */
    if (_spf != NULL) free(_spf);
    /* Build new cache */
    build_spf(n, &_spf);
    _spf_limit = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* Undefined for n=0, but safe */
    _ensure_spf(n);

    idx_t temp = n;
    seq_t sum = 0;
    idx_t cnt = 0;

    while (temp > 1) {
        idx_t p = _spf[temp];
        sum += (seq_t)p;
        cnt++;
        temp /= p;
    }

    if ((cnt & 1) == 0 && (sum & 1) == 0) return (seq_t)1;
    return (seq_t)0;
}
