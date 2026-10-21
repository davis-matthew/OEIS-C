/*
 * A249327  Rectangular array T(n,k) = f(n)*k^2, where f = A005117 (squarefree
 *          numbers); n, k >= 1; read by antidiagonals.
 * https://oeis.org/A249327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249327: a(n) is the n‑th entry of the rectangle
 * T(n,k) = f(n) * k^2, read by antidiagonals, where f is the squarefree
 * numbers A005117. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"

static seq_t *squarefree_values = NULL;  /* zero‑based: squarefree_values[0] = 1 */
static size_t squarefree_len   = 0;       /* number of values stored */

/* Ensure that squarefree_values contains at least 'need' values. */
static void ensure_squarefree(size_t need)
{
    if (squarefree_len >= need)
        return;

    /* Estimate an upper bound for the numeric value needed.
       Squarefree density ≈0.6079, so limit ≈ need/0.6 ≈ 1.7*need. */
    size_t limit = (size_t)(need * 2) + 100;
    if (limit < 10)
        limit = 10;

    /* Allocate array for sieve */
    uint8_t *sqfree = (uint8_t*)malloc((size_t)(limit + 1));
    if (!sqfree)
        abort();
    for (size_t i = 0; i <= limit; ++i)
        sqfree[i] = 1;                 /* mark all as squarefree initially */
    sqfree[0] = 0;                     /* 0 is not used */

    /* Mark numbers divisible by a square */
    size_t sqrt_limit = _floor_isqrt_u64(limit);
    for (size_t p = 2; p <= sqrt_limit; ++p) {
        size_t sq = p * p;
        for (size_t j = sq; j <= limit; j += sq)
            sqfree[j] = 0;
    }

    /* Reallocate array to hold up to 'need' values */
    seq_t *new_vals = (seq_t*)realloc(squarefree_values, need * sizeof(seq_t));
    if (!new_vals)
        abort();
    squarefree_values = new_vals;

    /* Collect squarefree numbers */
    size_t cnt = 0;
    for (size_t i = 1; i <= limit && cnt < need; ++i) {
        if (sqfree[i]) {
            squarefree_values[cnt++] = (seq_t)i;
        }
    }

    free(sqfree);
    squarefree_len = cnt;

    /* If we still haven't got enough values, extend further. */
    if (squarefree_len < need)
        ensure_squarefree(need);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for index 0, safety */

    /* Find the diagonal 'd' such that d*(d-1)/2 >= n.   */
    uint64_t n64 = (uint64_t)n;
    uint64_t disc = 1 + 8 * n64;
    uint64_t sqrt_disc = _floor_isqrt_u64(disc);

    uint64_t d = (1 + sqrt_disc) / 2;
    /* Correct small rounding errors. */
    while ((unsigned __int128)d * (d - 1) / 2 < (unsigned __int128)n64)
        ++d;

    /* Number of terms before diagonal d */
    unsigned __int128 prev_count = (unsigned __int128)(d - 1) * (d - 2) / 2;
    uint64_t offset = (uint64_t)(n64 - prev_count - 1); /* zero‑based */

    /* Map to (i,j) indices:  i = row = offset+1,  j = column = d - i. */
    size_t i = offset + 1;          /* i ≥ 1 */
    size_t j = d - i;               /* j ≥ 1 */

    /* Ensure we have the i‑th squarefree number. */
    ensure_squarefree(i);
    seq_t f_val = squarefree_values[i - 1]; /* first entry is 1, so index i-1 */
    seq_t k = (seq_t)j;
    seq_t val = f_val;
    val *= k;
    val *= k;                       /* multiply by j^2 */

    return val;
}
