/*
 * A359867  a(n) is the number of k > 0 such that n-1-2*k >= 0 and a(n-1-2*k)
 *          >= a(n-1-k) + a(n-1).
 * https://oeis.org/A359867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k > 0 such that n-1-2*k >= 0 and a(n-1-2*k) >=
 *            a(n-1-k) + a(n-1). Initial values: a(0)=0,a(1)=0,a(2)=0
 * Range:     u8 n <= 1160, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, realloc, abort */
#include <string.h>   /* memcpy */

/* Static memoisation cache */
static seq_t *cache = NULL;     /* holds a(0), a(1), … */
static size_t cap   = 0;        /* allocated capacity (number of elements) */
static idx_t next    = 0;       /* first index that has not yet been computed */

/* Make sure cache can store values up to n inclusive. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)cap > n) return;          /* already large enough */
    size_t newcap = (size_t)(n + 1);
    seq_t *newcache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!newcache) abort();               /* out of memory – stop the program */
    cache = newcache;
    cap   = newcap;
}

/* Computes a(n) with memoisation.  All values up to n are stored
   in the static cache so that future calls are fast. */
seq_t a(idx_t n)
{
    ensure_capacity(n);                   /* allocate if needed */

    /* Compute missing values up to n. */
    for (idx_t i = next; i <= n; ++i)
    {
        if (i < 3)                       /* a(0)=a(1)=a(2)=0 */
        {
            cache[i] = 0;
        }
        else
        {
            seq_t count = 0;
            for (idx_t k = 1; ; ++k)
            {
                idx_t two_k = 2 * k;
                if (two_k > i - 1)        /* n-1-2*k < 0  →  stop */
                    break;

                idx_t idx1 = i - 1 - two_k;   /* a(n-1-2*k) */
                idx_t idx2 = i - 1 - k;       /* a(n-1-k)   */

                if (cache[idx1] >= cache[idx2] + cache[i - 1])
                    ++count;
            }
            cache[i] = count;
        }
    }

    next = n + 1;                        /* all indices up to n are computed */
    return cache[n];
}
