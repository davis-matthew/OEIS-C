/*
 * A333017  Twice the total area of all (open or closed) Deutsch paths of
 *          length n.
 * https://oeis.org/A333017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = twice the total area of all Deutsch paths of length n
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Allocation helpers that work with both C and C++ */
static seq_t *alloc_seq(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n]();
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static void free_seq(seq_t *p)
{
    if (!p) return;
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache = NULL;      /* cached a(m) values */
    static size_t cacheCap = 0;
    static idx_t  cachedUpTo = 0;

    static seq_t *cntPrev = NULL;    /* number of paths ending at each height */
    static seq_t *a2Prev  = NULL;    /* twice the total area of paths ending at each height */
    static size_t prevSize = 0;      /* maximum height for previous length */

    /* Initialisation on the first call */
    if (cache == NULL) {
        cacheCap = 1;
        cache = alloc_seq(cacheCap);
        cache[0] = 0;                /* a(0) = 0 */
        cachedUpTo = 0;

        prevSize = 1;                /* heights 0 */
        cntPrev = alloc_seq(prevSize);
        a2Prev  = alloc_seq(prevSize);
        cntPrev[0] = 1;              /* empty path at height 0 */
        a2Prev[0]  = 0;              /* area = 0 */
    }

    /* Return cached value if available */
    if (n <= cachedUpTo)
        return cache[(size_t)n];

    /* Grow cache if necessary */
    if ((size_t)(n + 1) > cacheCap) {
        size_t newCap = cacheCap * 2;
        while (newCap <= (size_t)n)
            newCap *= 2;
        seq_t *tmp = alloc_seq(newCap);
        for (size_t i = 0; i < cacheCap; ++i)
            tmp[i] = cache[i];
        free_seq(cache);
        cache = tmp;
        cacheCap = newCap;
    }

    /* Compute a(t) for t = cachedUpTo+1 .. n */
    for (idx_t t = cachedUpTo + 1; t <= n; ++t) {
        size_t currSize = (size_t)t + 1;   /* heights 0..t */

        /* Allocate new state arrays */
        seq_t *cntNew = alloc_seq(currSize);
        seq_t *a2New  = alloc_seq(currSize);

        /* Build suffix sums over previous arrays */
        size_t spSize = prevSize + 1;      /* prevSize = t-1 */
        seq_t *suffC = alloc_seq(spSize);
        seq_t *suffA = alloc_seq(spSize);
        seq_t *suffH = alloc_seq(spSize);

        suffC[prevSize] = 0;
        suffA[prevSize] = 0;
        suffH[prevSize] = 0;
        for (size_t i = prevSize; i > 0; --i) {
            size_t idx = i - 1;
            suffC[i - 1] = cntPrev[idx] + suffC[i];
            suffA[i - 1] = a2Prev[idx] + suffA[i];
            suffH[i - 1] = cntPrev[idx] * (seq_t)idx + suffH[i];
        }

        seq_t sumA_total = 0;
        for (size_t h = 0; h < currSize; ++h) {
            seq_t newCnt = 0;
            seq_t newA2  = 0;

            /* Down transitions from all higher heights (> h) */
            if (h < prevSize) {
                seq_t downCnt = suffC[h + 1];
                seq_t downA   = suffA[h + 1] + downCnt * (seq_t)h + suffH[h + 1];
                newCnt += downCnt;
                newA2  += downA;
            }

            /* Up transition from height h-1 */
            if (h > 0) {
                size_t upIdx = h - 1;
                seq_t upCnt  = cntPrev[upIdx];
                seq_t upA    = a2Prev[upIdx] + upCnt * (seq_t)(2 * upIdx + 1);
                newCnt += upCnt;
                newA2  += upA;
            }

            cntNew[h] = newCnt;
            a2New[h]  = newA2;
            sumA_total += newA2;
        }

        cache[(size_t)t] = sumA_total;
        cachedUpTo = t;

        /* Free old arrays and move new ones into place */
        free_seq(cntPrev);
        free_seq(a2Prev);
        cntPrev = cntNew;
        a2Prev  = a2New;
        prevSize = currSize;

        free_seq(suffC);
        free_seq(suffA);
        free_seq(suffH);
    }

    return cache[(size_t)n];
}
