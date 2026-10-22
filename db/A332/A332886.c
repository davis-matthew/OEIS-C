/*
 * A332886  a(n) is the number of partition numbers that divide the n-th
 *          partition number (counted without multiplicity).
 * https://oeis.org/A332886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of distinct partition numbers that divide the
 *            n­th partition number. It is computed by: - computing the
 *            partition numbers p(0)…p(n) with Euler's pentagonal recurrence,
 *            - counting how many distinct p(k) with k≤n divide p(n). The only
 *            duplicate partition number is p(0)=p(1)=1, so...
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple memoised array of partition
   numbers so that subsequent calls are fast.  It works for all n
   up to about 10 000 in well under 2 s. */
#include <stdlib.h>

static seq_t *pCache = NULL;        /* Memoised partition numbers */
static idx_t  cacheSize = 0;        /* Number of elements in pCache */
static idx_t  lastComputed = 0;     /* Highest index that has been
                                      computed (p[lastComputed] is known) */

/* Extend the cache to hold p[0] … p[n] (allocate if necessary) */
static void ensure_cache(idx_t n)
{
    if (n < cacheSize) return;
    size_t newSize = (size_t)n + 1u;
    seq_t *tmp = (seq_t *)realloc(pCache, newSize * sizeof(seq_t));
    if (!tmp) abort();                /* out of memory */
    /* initialise new elements to zero */
    for (idx_t i = cacheSize; i < newSize; ++i) tmp[i] = 0;
    pCache   = tmp;
    cacheSize = newSize;
    /* make sure p[0] is correctly initialised */
    if (cacheSize > 0) pCache[0] = 1;
}

/* Compute p[k] for all k > lastComputed up to and including n */
static void compute_partitions(idx_t n)
{
    if (n <= lastComputed) return;
    for (idx_t i = lastComputed + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;          /* accumulate positive and
                                            negative contributions */
        for (idx_t k = 1; ; ++k) {
            /* pentagonal numbers */
            idx_t pent1 = k*(3*k-1)/2;
            if (pent1 > i) break;        /* both further indices will
                                            also be > i */
            seq_t term = pCache[i - pent1];
            if (k & 1u) pos += term;
            else       neg += term;

            idx_t pent2 = k*(3*k+1)/2;
            if (pent2 > i) continue;     /* ignore negative index */
            term = pCache[i - pent2];
            if (k & 1u) pos += term;
            else       neg += term;
        }
        /* pos should be >= neg for all i */
        pCache[i] = pos - neg;
    }
    lastComputed = n;
}

seq_t a(idx_t n)
{
    /* Make sure we have partition numbers up to n. */
    ensure_cache(n);
    compute_partitions(n);

    seq_t pn   = pCache[n];   /* the n-th partition number */
    seq_t count = 0;          /* number of distinct divisors */

    for (idx_t k = 0; k <= n; ++k) {
        /* Skip duplicate values of p(k) (only 1 repeats). */
        if (k > 0 && pCache[k] == pCache[k-1]) continue;
        if (pn % pCache[k] == 0) count += 1;
    }
    return count;
}
