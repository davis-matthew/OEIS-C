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
 * Helpers:   is_squarefree, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the maximum of the n‑th maximal run of nonsquarefree numbers.
   It scans integers, groups consecutive nonsquarefree numbers into
   maximal runs, and stores the largest number of each run. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

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
