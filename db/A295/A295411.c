/*
 * A295411  Number of n X 3 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1, 2 or 3 1s.
 * https://oeis.org/A295411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 28*a(n-3) + 3*a(n-4) + 49*a(n-5) + 33*a(n-6) -
 *            34*a(n-7) - 22*a(n-8); a(1)=4, a(2)=36, a(3)=234, a(4)=1534,
 *            a(5)=10291, a(6)=68613, a(7)=457178, a(8)=3048314
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based cache of computed terms         */
static idx_t  computed = 0;   /* largest n for which cache[n] is valid   */
static idx_t  cap = 0;        /* allocated size of cache                */

/* Ensure the cache can hold indices up to n (inclusive).  The first
   eight terms are inserted on the first allocation.                     */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL)
    {
        /* Allocate space for indices 0 … 8 (index 0 unused). */
        cap = 9;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = 0;                 /* unused placeholder */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)234;
        cache[4] = (seq_t)1534;
        cache[5] = (seq_t)10291;
        cache[6] = (seq_t)68613;
        cache[7] = (seq_t)457178;
        cache[8] = (seq_t)3048314;
        computed = 8;
    }
    if ((idx_t)(n + 1) > cap)
    {
        idx_t new_cap = cap * 2;
        if (new_cap < (idx_t)(n + 1))
            new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);
        cache = new_cache;
        cap = new_cap;
    }
}

/* Compute a(n) for n ≥ 1, memoising the results. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* sequence defined for n ≥ 1 */

    if (n <= computed) return cache[n];

    ensure_capacity(n);

    idx_t start = (computed < 8 ? 9 : computed + 1);
    for (idx_t i = start; i <= n; ++i)
    {
        seq_t val = 0;
        val += (seq_t)6  * cache[i-1];
        val += (seq_t)28 * cache[i-3];
        val += (seq_t)3  * cache[i-4];
        val += (seq_t)49 * cache[i-5];
        val += (seq_t)33 * cache[i-6];
        val -= (seq_t)34 * cache[i-7];
        val -= (seq_t)22 * cache[i-8];
        cache[i] = val;
    }
    computed = n;
    return cache[n];
}
