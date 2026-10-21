/*
 * A294237  Sum of the larger parts of the partitions of n into two parts with
 *          smaller part nonsquarefree.
 * https://oeis.org/A294237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (n - i) * (1 - mu(i)^2)
 * Range:     u8 n <= 47, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static size_t _sqrf_limit = 0;          /* largest index with computed data */
static bool *_sqrf = NULL;              /* true = squarefree, false = not squarefree */

/* Precompute squarefree information up to `upto`.  `upto` is inclusive. */
static void _ensure_squarefree_precomp(idx_t upto)
{
    if (upto <= (idx_t)_sqrf_limit)
        return;

    size_t sz = (size_t)upto + 1;              /* array indices 0 .. upto */
    bool *arr = (bool *)malloc(sz * sizeof(bool));
    if (!arr) {                                 /* graceful exit on OOM */
        exit(1);
    }

    /* Initially mark everything as squarefree. */
    for (size_t i = 0; i < sz; ++i)
        arr[i] = true;

    /* Cross off multiples of squares. */
    for (idx_t p = 2; p <= upto / p; ++p) {     /* p*p may overflow -> stop at p <= upto/p */
        idx_t square = p * p;
        for (idx_t j = square; j <= upto; j += square)
            arr[j] = false;
    }

    free(_sqrf);
    _sqrf      = arr;
    _sqrf_limit = (size_t)upto;
}

/* OEIS sequence A294237.  Returns a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    idx_t m = n / 2;                     /* floor(n/2) */
    _ensure_squarefree_precomp(m);

    seq_t sum = 0;
    for (idx_t i = 1; i <= m; ++i) {
        if (!_sqrf[i])                   /* i is NOT squarefree (mu(i) == 0) */
            sum += (seq_t)(n - i);
    }
    return sum;
}
