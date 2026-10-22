/*
 * A357717  Number of ways to write n as an ordered sum of nine positive
 *          Fibonacci numbers (with a single type of 1).
 * https://oeis.org/A357717
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   number of ways to write n as an ordered sum of nine positive
 *            Fibonacci numbers * (1, 2, 3, 5, 8, …)
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/*  Dynamic programming cache: 10 rows for 0–9 parts, up to largest n     */
/* --------------------------------------------------------------------- */
static idx_t          _curmax   = 0;      /* largest n processed so far          */
static seq_t         * _dp[10]   = {NULL};/* pointer array of DP rows (k parts)  */
static idx_t        * _fib      = NULL;    /* Fibonacci numbers up to current max */
static size_t         _fibcnt   = 0;       /* number of Fibonacci values         */

/* --------------------------------------------------------------------- */
/*  Free all cached memory                                               */
/* --------------------------------------------------------------------- */
static void _free_cache(void)
{
    if (_dp[0] != NULL) {
        for (int k = 0; k < 10; ++k)
            free(_dp[k]);
        for (int k = 0; k < 10; ++k)
            _dp[k] = NULL;
    }
    free(_fib);
    _fib      = NULL;
    _fibcnt   = 0;
    _curmax   = 0;
}

/* --------------------------------------------------------------------- */
/*  Recompute DP tables up to newmax (newmax > _curmax)                   */
/* --------------------------------------------------------------------- */
static void _recompute(idx_t newmax)
{
    _free_cache();

    /* ----- compute Fibonacci numbers (1, 2, 3, 5, …) up to newmax ---- */
    size_t cap = 32;
    _fib = (idx_t*)malloc(cap * sizeof(idx_t));
    idx_t a = 1, b = 2;
    _fibcnt = 0;
    _fib[_fibcnt++] = a;                /* 1  */
    if (newmax >= 2)
        _fib[_fibcnt++] = b;            /* 2  */
    while (1) {
        idx_t c = a + b;
        if (c > newmax)
            break;
        a = b;  b = c;
        if (_fibcnt == cap) {
            cap <<= 1;
            _fib = (idx_t*)realloc(_fib, cap * sizeof(idx_t));
        }
        _fib[_fibcnt++] = c;
    }

    /* ----------------- allocate DP tables -------------------------- */
    for (int k = 0; k < 10; ++k)
        _dp[k] = (seq_t*)calloc((size_t)(newmax + 1), sizeof(seq_t));

    /* base case: 0 parts sum to 0 in exactly one way */
    _dp[0][0] = (seq_t)1;

    /* DP: compute for k = 1 … 9 parts */
    for (int k = 1; k <= 9; ++k) {
        seq_t *prev = _dp[k-1];
        seq_t *cur  = _dp[k];
        for (idx_t s = 1; s <= newmax; ++s) {
            seq_t total = (seq_t)0;
            for (size_t i = 0; i < _fibcnt && _fib[i] <= s; ++i)
                total += prev[(size_t)(s - _fib[i])];
            cur[(size_t)s] = total;
        }
    }

    _curmax = newmax;
}

/* --------------------------------------------------------------------- */
/*  Public sequence function                                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The minimal sum of nine positive Fibonacci numbers (1×9) is 9. */
    if (n < 9)
        return (seq_t)0;

    if (n > _curmax)
        _recompute(n);

    return _dp[9][(size_t)n];
}
