/*
 * A280260  Partial products of A280258.
 * https://oeis.org/A280260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} A280258(i) where A280258(i) = Sum_{d|i}
 *            (Product of totatives of d)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   _gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following code relies only on ordinary C operators and dynamic
   storage.  It works for any seq_t that supports the indicated
   arithmetic operators (including arbitrary‑precision integer classes). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/_gcd_idx.h"

/* Euclidean GCD for idx_t values. */

/* Static cache arrays.  Index 0 is unused. */
static seq_t *pxi   = NULL;   /* product of totatives of m */
static seq_t *a258  = NULL;   /* A280258(n) */
static seq_t *ans   = NULL;   /* a(n) partial product */
static size_t maxN  = 0;      /* largest n that has been fully computed */

/* Resize the internal caches to cover 1..newN. */
static void ensure_size(size_t newN)
{
    if (newN <= maxN)
        return;

    /* Allocate or grow the pxi array. */
    if (pxi == NULL) {
        pxi = (seq_t *)calloc(newN + 1, sizeof(seq_t));
    } else {
        pxi = (seq_t *)realloc(pxi, (newN + 1) * sizeof(seq_t));
        memset(pxi + maxN + 1, 0, (newN - maxN) * sizeof(seq_t));
    }

    /* Allocate or grow the a258 array. */
    if (a258 == NULL) {
        a258 = (seq_t *)calloc(newN + 1, sizeof(seq_t));
    } else {
        a258 = (seq_t *)realloc(a258, (newN + 1) * sizeof(seq_t));
        memset(a258 + maxN + 1, 0, (newN - maxN) * sizeof(seq_t));
    }

    /* Allocate or grow the ans array. */
    if (ans == NULL) {
        ans = (seq_t *)calloc(newN + 1, sizeof(seq_t));
    } else {
        ans = (seq_t *)realloc(ans, (newN + 1) * sizeof(seq_t));
        memset(ans + maxN + 1, 0, (newN - maxN) * sizeof(seq_t));
    }

    /* Compute pxi(m) for new indices m = maxN+1 .. newN. */
    for (size_t m = maxN + 1; m <= newN; ++m) {
        seq_t prod = 1;
        for (idx_t k = 1; k <= (idx_t)m; ++k) {
            if (_gcd_idx(k, (idx_t)m) == 1)
                prod *= (seq_t)k;
        }
        pxi[m] = prod;
    }

    /* Recompute A280258(n) for all n up to newN. */
    for (size_t i = 1; i <= newN; ++i)
        a258[i] = 0;

    for (size_t d = 1; d <= newN; ++d) {
        seq_t val = pxi[d];
        for (size_t j = d; j <= newN; j += d)
            a258[j] += val;
    }

    /* Compute the partial products a(n). */
    if (newN >= 1)
        ans[1] = a258[1];
    for (size_t i = 2; i <= newN; ++i)
        ans[i] = ans[i - 1] * a258[i];

    maxN = newN;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1 */
    ensure_size((size_t)n);
    return ans[(size_t)n];
}
