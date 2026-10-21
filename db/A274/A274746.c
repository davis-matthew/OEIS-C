/*
 * A274746  Number of n X 5 0..2 arrays with no element equal to any value at
 *          offset (-1,-2) (0,-1) or (-1,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A274746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 14*a(n-3) + 10*a(n-4) - 2*a(n-5) for n > 6
 *            Initial values: a(1)=8, a(2)=60, a(3)=518, a(4)=4554,
 *            a(5)=40242, a(6)=355504 (This function uses memoisation and
 *            full‑precision arithmetic for arbitrary seq_t types.)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* OEIS sequence A274746 */
seq_t a(idx_t n)
{
    if (n == 0)          /* not defined for n=0, but return 0 safely */
        return (seq_t)0;

    /* static cache and its capacity, initialised only once */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;
    static bool   init  = false;

    /* initial allocation and values */
    if (!init) {
        cap = 7;                                 /* indices 0..6 */
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)60;
        cache[3] = (seq_t)518;
        cache[4] = (seq_t)4554;
        cache[5] = (seq_t)40242;
        cache[6] = (seq_t)355504;
        init = true;
    }

    /* grow cache if the request exceeds current capacity */
    if (n >= cap) {
        idx_t newcap = n + 1;                     /* store indices 0..n */
        seq_t *newcache = (seq_t*)calloc(newcap, sizeof(seq_t));
        memcpy(newcache, cache, cap * sizeof(seq_t));
        free(cache);
        cache = newcache;
        cap = newcap;
    }

    /* return early for the first six terms */
    if (n <= 6)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = 7; i <= n; ++i) {
        if (cache[i] != (seq_t)0)   /* already computed by a previous call */
            continue;
        seq_t v1 = cache[i-1];
        seq_t v2 = cache[i-3];
        seq_t v3 = cache[i-4];
        seq_t v4 = cache[i-5];

        seq_t val = v1 * (seq_t)9;
        val -= v2 * (seq_t)14;
        val += v3 * (seq_t)10;
        val -= v4 * (seq_t)2;
        cache[i] = val;
    }

    return cache[n];
}
