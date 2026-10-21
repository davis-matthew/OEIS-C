/*
 * A276206  a(0) = a(1) = a(2) = a(3) = 0. For n>3 a(n) is the smallest
 *          nonnegative integer such that there is no arithmetic progression
 *          i,j,k,m,n (of length 5) such that a(i)+a(j)+a(k)+a(m) = a(n).
 * https://oeis.org/A276206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4852, u16 n <= 9056, u32 n <= 9056, u64 n <= 9056,
 *            u128 n <= 9056, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A276206: a(n) is the smallest non‑negative integer not representable
   as a(i)+a(j)+a(k)+a(m) for any arithmetic progression i,j,k,m,n
   of length 5.  The values for n ≤ 3 are 0. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Comparator used only if sorting is needed (not used in final version). */
static int seq_cmp(const void *p, const void *q)
{
    const seq_t *x = (const seq_t *)p;
    const seq_t *y = (const seq_t *)q;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Compute a(n).  Cached results allow any call order. */
seq_t a(idx_t n)
{
    /* Static cache of computed values. */
    static seq_t *cache = NULL;   /* array of a[0] .. a[len-1] */
    static size_t len = 0;        /* number of values stored */
    static size_t cap = 0;        /* allocated capacity */

    /* Release already computed terms above n?  We keep all to handle
       arbitrary call order.  Just grow if needed. */
    if ((size_t)n + 1 > len) {
        size_t need = (size_t)n + 1;
        if (need > cap) {
            size_t newcap = cap ? cap * 2 : 4;
            while (newcap < need) newcap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();   /* allocation failed */
            cache = tmp;
            cap = newcap;
        }
        /* Compute missing terms up to n. */
        for (size_t i = len; i <= (size_t)n; ++i) {
            if (i <= 3) {          /* initial values */
                cache[i] = (seq_t)0;
                continue;
            }
            /* d ranges from 1 to i/4 inclusive (integer division). */
            size_t dmax = i / 4;
            /* visited array up to dmax (inclusive), size dmax+1. */
            bool *visited = (bool *)calloc(dmax + 1, sizeof(bool));
            if (!visited) abort();
            for (size_t d = 1; d <= dmax; ++d) {
                size_t base = i - 4 * d;
                seq_t s = cache[base];
                s += cache[base + d];
                s += cache[base + 2 * d];
                s += cache[base + 3 * d];
                /* Mark if s <= dmax. */
                if (s <= (seq_t)dmax) {
                    visited[(size_t)s] = true;
                }
            }
            /* Find lowest non‑negative integer not in visited. */
            seq_t missing = 0;
            while (missing <= (seq_t)dmax && visited[(size_t)missing]) {
                ++missing;
            }
            free(visited);
            cache[i] = missing;
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
