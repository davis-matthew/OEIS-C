/*
 * A376164  Maximum of the n-th maximal run of nonsquarefree numbers
 *          (increasing by 1 at a time).
 * https://oeis.org/A376164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9855 (2026-11-01)
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the maximum of the n‑th maximal run of nonsquarefree numbers.
   It scans integers, groups consecutive nonsquarefree numbers into
   maximal runs, and stores the largest number of each run. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* cache the run maxima */
    static seq_t *cache = NULL;   /* array of run maxima */
    static size_t  cap   = 0;      /* current capacity of the array */
    static idx_t   till  = 0;      /* number of run maxima already computed */
    static idx_t   cur   = 2;      /* next integer to examine */

    /* initial allocation */
    if (cache == NULL) {
        cap   = 64;
        cache = seq_alloc(cap);
        if (cache == NULL) return (seq_t)0; /* allocation failed */
    }

    /* generate runs until we have at least n of them */
    while (till < n) {
        /* skip square‑free numbers to find start of a nonsquarefree run */
        while (cur != 0 && is_squarefree((seq_t)cur))
            cur++;

        /* if overflow reached, stop (unlikely for n <= 10000) */
        if (cur == 0)
            break;

        /* start of a run; record its upper bound as we extend it */
        seq_t run_max = (seq_t)cur;
        cur++;

        /* extend the run while numbers stay nonsquarefree */
        while (cur != 0 && !is_squarefree((seq_t)cur)) {
            run_max = (seq_t)cur;
            cur++;
        }

        /* ensure enough room in cache */
        if (till >= cap) {
            size_t newcap = cap * 2;
            seq_t *tmp = seq_realloc(cache, cap, newcap);
            if (tmp == NULL) return (seq_t)0;    /* allocation failed */
            cache = tmp;
            cap   = newcap;
        }

        cache[till++] = run_max;
    }

    /* if n is larger than the number of runs found due to overflow,
       return zero; this situation occurs only when cur overflows. */
    if (n == 0 || n > till)
        return (seq_t)0;

    return cache[n - 1];
}
