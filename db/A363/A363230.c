/*
 * A363230  Number of partitions of n with rank 3 or higher (the rank of a
 *          partition is the largest part minus the number of parts).
 * https://oeis.org/A363230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k>=1} (-1)^(k-1) * p(n - k*(3*k+5)/2), where p(m) is
 *            the partition number and p(m)=0 for m<0. The code computes the
 *            partition numbers once and reuses them
 * Range:     u8 n <= 20, u16 n <= 48, u32 n <= 135, u64 n <= 429,
 *            u128 n <= 1480, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* static caches for partition numbers p[0..max] and the sequence a[0..max] */
static seq_t *partitions = NULL;
static seq_t *seq_cache  = NULL;
static idx_t  max_cached = 0;

/* Reallocate an array of seq_t, copying old contents.  Works in C
   and C++ (uses new/delete for C++ to avoid destroying non‑POD
   elements). */
static seq_t *realloc_seq_t(seq_t *old_arr, size_t old_cap, size_t new_cap)
{
    seq_t *new_arr;
#if defined(__cplusplus)
    new_arr = (seq_t *)operator new(new_cap * sizeof(seq_t));
#else
    new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_arr) return NULL;           /* allocation failed */

    /* copy existing values */
    for (size_t i = 0; i < old_cap; ++i) new_arr[i] = old_arr[i];

    /* free the old array */
    if (old_arr) {
#if defined(__cplusplus)
        operator delete[](old_arr);
#else
        free(old_arr);
#endif
    }
    return new_arr;
}

/* Ensure that partitions[0..n] and seq_cache[0..n] are computed. */
static void ensure_cache(idx_t n)
{
    if (n <= max_cached) return;             /* already have what we need */

    size_t new_cap = (size_t)n + 1;           /* indices 0..n inclusive */

    /* Allocate or grow the partitions array */
    if (!partitions) {
        partitions = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!partitions) return;             /* out of memory */
    } else if (new_cap > (size_t)max_cached + 1) {
        seq_t *tmp = realloc_seq_t(partitions, (size_t)max_cached + 1, new_cap);
        if (!tmp) return;                    /* out of memory */
        partitions = tmp;
    }

    /* Allocate or grow the sequence cache array */
    if (!seq_cache) {
        seq_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!seq_cache) return;
    } else if (new_cap > (size_t)max_cached + 1) {
        seq_t *tmp = realloc_seq_t(seq_cache, (size_t)max_cached + 1, new_cap);
        if (!tmp) return;
        seq_cache = tmp;
    }

    /* Initialise base case if this is the first allocation */
    if (max_cached == 0) {
        partitions[0] = (seq_t)1;            /* p(0)=1 */
        seq_cache[0]  = (seq_t)0;            /* a(0)=0 (unused for n>=1) */
    }

    /* Compute new partition numbers p[i] for i=max_cached+1 .. n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            if (k & 1) pos += partitions[i - pent1];
            else       neg += partitions[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= i) {
                if (k & 1) pos += partitions[i - pent2];
                else       neg += partitions[i - pent2];
            }
        }
        partitions[i] = pos - neg;
    }

    /* Compute sequence values a[i] for i=max_cached+1 .. n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t gk = k * (3 * k + 5) / 2;   /* generalized pentagonal */
            if (gk > i) break;
            if (k & 1) pos += partitions[i - gk];
            else       neg += partitions[i - gk];
        }
        seq_cache[i] = pos - neg;
    }

    max_cached = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset 1 – values below 1 are irrelevant */
    ensure_cache(n);
    return seq_cache[n];
}
