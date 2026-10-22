/*
 * A397687  a(n) is the least k >= 1 such that Sum_{i = 0..n} A000005(k + i) /
 *          (n + 1) is an integer.
 * https://oeis.org/A397687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function a(n) computes the OEIS sequence A397687.
    a(n) = the least k ≥ 1 such that
           ( Σ_{i=0..n} d(k+i) ) mod (n+1) == 0
    where d(m) = A000005(m) = number of positive divisors of m. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global state: divisor counts and prefix sums up to _max_limit. */
static size_t _max_limit  = 0;          /* largest index for which data are valid */
static uint32_t * _divcnt = NULL;       /* divisor counts d[0 .. _max_limit]   */
static seq_t   * _prefix  = NULL;       /* prefix sums (_prefix[i] = Σ_{t=1..i} d[t]) */

/* Ensure that divisor counts and prefix sums exist up to 'limit'.
   Recomputes all values from scratch when extending. */
static void
_ensure_limit(size_t limit)
{
    if (limit <= _max_limit)
        return;                      /* already sufficient */

    /* Reallocate arrays to new size (include index 0). */
    _divcnt = (uint32_t *)realloc(_divcnt, (limit + 1) * sizeof(uint32_t));
    _prefix = (seq_t   *)realloc(_prefix, (limit + 1) * sizeof(seq_t));

    /* Initialise all entries to zero. */
    for (size_t i = 0; i <= limit; ++i)
        _divcnt[i] = 0;

    /* Compute divisor counts via the standard O(n log n) sieve. */
    for (size_t i = 1; i <= limit; ++i) {
        for (size_t j = i; j <= limit; j += i)
            ++_divcnt[j];
    }

    /* Build prefix sums. */
    _prefix[0] = 0;
    for (size_t i = 1; i <= limit; ++i)
        _prefix[i] = _prefix[i - 1] + (seq_t)_divcnt[i];

    _max_limit = limit;
}

/*------------------------------------------------------------------*/
/*  Main sequence function: a(n)                                      */
/*------------------------------------------------------------------*/
seq_t
a(idx_t n)
{
    size_t N = (size_t)n;     /* block length L = n + 1 */
    size_t L = N + 1;

    /* We need divisor data up to at least N+1 for k = 1. */
    _ensure_limit(L);

    /* Current window sum for k=1: Σ_{i=1..L} d[i] */
    seq_t sum = _prefix[L];

    /* Quick check for k = 1 */
    if ((sum % (seq_t)L) == 0)
        return (seq_t)1;

    /* Search for subsequent k. */
    size_t k = 1;
    while (true) {
        ++k;

        /* Ensure divisor data covers k + N (end of the window). */
        if (k + N > _max_limit)
            _ensure_limit(k + N);

        /* Update window sum: add new divisor d[k+N], subtract old d[k-1]. */
        sum += (seq_t)_divcnt[k + N];
        sum -= (seq_t)_divcnt[k - 1];

        if ((sum % (seq_t)L) == 0)
            return (seq_t)k;
    }
}
