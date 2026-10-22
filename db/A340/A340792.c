/*
 * A340792  List in which n appears ceiling(d(n)/2) = A038548(n) times, where
 *          d(n) is the number of divisors of n.
 * https://oeis.org/A340792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340792: each integer m appears ceil(d(m)/2) times,
   where d(m) is the number of positive divisors of m.
   The function a(n) returns the n‑th term of this multiset (a(1)=1). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Statically maintained data structures for on‑demand growth.  */
static idx_t *cum = NULL;          /* cum[i] = Σ_{k=1..i} ceil(d(k)/2)  */
static uint32_t *divs = NULL;      /* divisor counts up to current limit   */
static idx_t max_limit = 0;        /* largest index such that cum is valid */

/* Ensure that cum[] and divs[] have been computed for all i ≤ limit. */
static void ensure_to(idx_t limit)
{
    if (limit <= max_limit)
        return;

    /* Allocate space for indices 0 … limit. */
    size_t sz = (size_t)(limit + 1);
    cum   = (idx_t*)realloc(cum,   sz * sizeof(idx_t));
    divs  = (uint32_t*)realloc(divs, sz * sizeof(uint32_t));
    if (!cum || !divs)
        abort();

    /* Compute divisor counts by a simple sieve. */
    for (idx_t i = 1; i <= limit; ++i)
        divs[i] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        for (idx_t j = i; j <= limit; j += i)
            divs[j] += 1;

    /* Build cumulative sums of ceil(divs[i]/2). */
    cum[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        idx_t add = (divs[i] + 1) / 2;   /* ceil(divs[i]/2) */
        cum[i] = cum[i - 1] + add;
    }

    max_limit = limit;
}

/* Return the n-th term of A340792 (a(1)=1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n = 0 */

    /* Initialise the tables if this is the first call. */
    if (!cum)
        ensure_to(1);

    /* Extend the precomputation until the cumulative total covers n. */
    while (cum[max_limit] < n) {
        idx_t new_limit = max_limit * 2;
        if (new_limit < max_limit + 1)   /* avoid overflow on small max_limit */
            new_limit = max_limit + 1;
        ensure_to(new_limit);
    }

    /* Binary search for the smallest m with cum[m] >= n. */
    idx_t lo = 1, hi = max_limit;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (cum[mid] >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return (seq_t)lo;   /* conversion to arbitrary‑precision type is implicit */
}
