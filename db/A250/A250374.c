/*
 * A250374  Number of length 1+6 0..n arrays with every seven consecutive
 *          terms having the maximum of some three terms equal to the minimum
 *          of the remaining four terms.
 * https://oeis.org/A250374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) * +
 *            21*a(n-5) - 7*a(n-6) + a(n-7) for n > 7 * initial values: *
 *            a(1)=93, a(2)=1382, a(3)=8964, a(4)=37385, * a(5)=118621,
 *            a(6)=312578, a(7)=720112
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 32, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Small table for the first 7 terms */
static const seq_t _a_init[8] = {
    0ULL,          /* unused, 0th index */
    93ULL,         /* a(1) */
    1382ULL,       /* a(2) */
    8964ULL,       /* a(3) */
    37385ULL,      /* a(4) */
    118621ULL,     /* a(5) */
    312578ULL,     /* a(6) */
    720112ULL      /* a(7) */
};

/* Cached sequence: indices 1 .. _a_cache_size are valid */
static seq_t * _a_cache = NULL;
static idx_t  _a_cache_size = 0;

/* Ensure that _a_cache contains values up to n (inclusive). */
static void _ensure_a_cache(idx_t n)
{
    if (n <= _a_cache_size)
        return;                           /* already computed */

    /* Allocate space for indices 0 .. n   (index 0 unused) */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(_a_cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        abort();                           /* out of memory */
    }
    _a_cache = tmp;

    if (_a_cache_size == 0) {              /* first allocation */
        for (idx_t i = 1; i <= 7 && i <= n; ++i)
            _a_cache[i] = _a_init[i];
        _a_cache_size = (n < 7) ? n : 7;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = _a_cache_size + 1; i <= n; ++i)
    {
        seq_t t0 = _a_cache[i-1];
        seq_t t1 = _a_cache[i-2];
        seq_t t2 = _a_cache[i-3];
        seq_t t3 = _a_cache[i-4];
        seq_t t4 = _a_cache[i-5];
        seq_t t5 = _a_cache[i-6];
        seq_t t6 = _a_cache[i-7];

        /* Positive part */
        seq_t val = 7 * t0;
        val += 35 * t2;
        val += 21 * t4;
        val += t6;

        /* Negative part */
        val -= 21 * t1;
        val -= 35 * t3;
        val -= 7 * t5;

        _a_cache[i] = val;
    }

    _a_cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1) {
        /* Undefined below the offset; return 0 for safety */
        return 0;
    }
    if (n <= 7) {
        return _a_init[n];
    }
    _ensure_a_cache(n);
    return _a_cache[n];
}
