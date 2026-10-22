/*
 * A322837  Number of positive integers less than n with fewer distinct prime
 *          factors than n.
 * https://oeis.org/A322837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 321, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by sieving distinct prime factors (ω) and cumulative counts. */

#include <stdlib.h>
#include <stdint.h>

/* Static state for memoisation */
static idx_t    _maxN    = 0;          /* largest n processed so far            */
static uint8_t * _omega   = NULL;      /* ω[i] for i = 0 … _maxN                */
static uint32_t * _prefix = NULL;      /* flattened 2‑D array [row][col]        */
static size_t    _prefixRows = 0;      /* number of ω rows (maxΩ+1)             */
static size_t    _prefixCols = 0;      /* number of columns = _maxN+1           */

/* Ensure data is computed up to and including n */
static void _ensure(idx_t n)
{
    if (n <= _maxN)
        return;

    /* free old data */
    free(_omega);
    free(_prefix);

    /* allocate new ω array (size n+1 to include index n) */
    _omega = (uint8_t *)malloc((size_t)(n + 1) * sizeof(uint8_t));
    if (!_omega)
        exit(1);                       /* out of memory */
    memset(_omega, 0, (size_t)(n + 1) * sizeof(uint8_t));

    /* simple sieve for ω */
    for (idx_t i = 2; i <= n; ++i) {
        if (_omega[i] == 0) {           /* i is prime                */
            for (idx_t j = i; j <= n; j += i)
                ++_omega[j];
        }
    }

    /* find maximum ω value */
    uint8_t maxOmega = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (_omega[i] > maxOmega)
            maxOmega = _omega[i];

    _prefixRows = (size_t)(maxOmega + 1);
    _prefixCols = (size_t)(n + 1);
    _prefix = (uint32_t *)malloc(_prefixRows * _prefixCols * sizeof(uint32_t));
    if (!_prefix)
        exit(1);

    /* initialise first column (zero) */
    for (size_t r = 0; r < _prefixRows; ++r)
        _prefix[r * _prefixCols + 0] = 0;

    /* build cumulative counts */
    for (idx_t i = 1; i <= n; ++i) {
        for (size_t r = 0; r < _prefixRows; ++r) {
            uint32_t prev = _prefix[r * _prefixCols + (i - 1)];
            _prefix[r * _prefixCols + i] = prev + (_omega[i] == (uint8_t)r ? 1 : 0);
        }
    }

    _maxN = n;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    _ensure(n);

    uint8_t omega_n = _omega[n];
    seq_t result = 0;
    for (uint8_t r = 0; r < omega_n; ++r) {
        result += _prefix[(size_t)r * _prefixCols + (n - 1)];
    }
    return result;
}
