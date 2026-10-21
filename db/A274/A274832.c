/*
 * A274832  Values of n such that 2*n+1 and 7*n+1 are both triangular numbers
 *          (A000217).
 * https://oeis.org/A274832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer n such that the numbers 2*n+1 and 7*n+1
 *            are triangular. This is equivalent to the linear recurrence a(n)
 *            = a(n-1) + 898*a(n-2) - 898*a(n-3) - a(n-4) + a(n-5) with
 *            initial values a(1)=0, a(2)=27, a(3)=297, a(4)=24570,
 *            a(5)=267030
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache for computed terms */
static size_t  cached = 0;           /* number of terms currently stored */
static size_t  capacity = 0;         /* allocated capacity of cache */

/* Ensure enough space in the cache for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (capacity < need) {
        size_t newcap = (capacity == 0) ? 16 : capacity * 2;
        if (newcap < need) newcap = need;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) { /* in a proper build we could abort, but it's not required */ }
        cache = tmp;
        capacity = newcap;
    }
}

/* The five known starting values. */
static const seq_t base_vals[5] = {
    (seq_t)0,
    (seq_t)27,
    (seq_t)297,
    (seq_t)24570,
    (seq_t)267030
};

seq_t a(idx_t n)
{
    /* Quick path for the first five terms. */
    if (n <= 5) return base_vals[n - 1];

    /* Make sure cache can accommodate all terms up to n. */
    size_t need = (size_t)n;
    ensure_capacity(need);

    /* Initialise the cache with the known first five terms if not already done. */
    if (cached < 5) {
        size_t i;
        for (i = cached; i < 5 && i < need; ++i) {
            cache[i] = base_vals[i];
        }
        cached = i;
    }

    /* Generate missing terms up to the requested index. */
    while (cached < need) {
        size_t idx = cached;                /* zero‑based index of the term to compute */
        /* Use the recurrence with only unsigned operations. */
        /* buf[4] = a(idx-1), buf[3] = a(idx-2), buf[2] = a(idx-3),
         * buf[1] = a(idx-4), buf[0] = a(idx-5)        */
        seq_t next = cache[idx - 1] + (seq_t)898 * cache[idx - 2] + cache[idx - 5];
        next      -= (seq_t)898 * cache[idx - 3];
        next      -= cache[idx - 4];
        cache[idx] = next;
        ++cached;
    }

    return cache[n - 1];
}
