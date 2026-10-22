/*
 * A384489  Consecutive states of the linear congruential pseudo-random number
 *          generator 392314069 * s mod 2^32 when started at s=1.
 * https://oeis.org/A384489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(n) = 392314069 * a(n-1) mod 2^32 for n ≥ 2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Constants used in the recurrence */
static const seq_t _MULT  = (seq_t)392314069ULL;
static const seq_t _MASK  = ((((seq_t)1) << 32) - 1);

/* Cache of already computed terms: a[0] = a(1), a[1] = a(2), ... */
static seq_t * _cache = NULL;
static idx_t  _cache_size = 0;

/* Return the n‑th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* Undefined for n < 1 */
    if (n > _cache_size) {
        idx_t new_size = n;
        seq_t *new_cache;
        if (_cache == NULL)
            new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        else
            new_cache = (seq_t*)realloc(_cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();

        /* Initialize the cache if we are starting from scratch */
        if (_cache_size == 0) {
            new_cache[0] = (seq_t)1;
            _cache_size = 1;
        }

        /* Compute new terms */
        for (idx_t i = _cache_size; i < new_size; ++i) {
            seq_t prod = _MULT * new_cache[i-1];
            new_cache[i] = prod & _MASK;
        }

        _cache = new_cache;
        _cache_size = new_size;
    }
    return _cache[n-1];
}
