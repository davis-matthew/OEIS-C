/*
 * A268306  The number of even permutations p of 1,2,...,n such that
 *          -1<=p(i)-i<=2 for i=1,2,...,n.
 * https://oeis.org/A268306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 74, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268306:
 * a(n) = A133872(n)/2 + A000073(n+2)/2
 * Recurrence for n ≥ 7:
 *   a(n) = 2*a(n-1) – a(n-2) + 2*a(n-3) – a(n-4) – a(n-6)
 * Initial values (n = 0..6) used for memoisation:
 *   a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=4, a(5)=7, a(6)=12
 * The function is memoised so repeated calls of any order remain
 * O(n) overall for the largest n requested.
 */
#include <stdlib.h>

static seq_t *cache = NULL;       /* array of computed a(n) */
static idx_t cache_cap = 0;       /* allocated size of cache */
static idx_t last_comp = 0;       /* highest index computed so far */

/* ensures cache has room for indices up to idx */
static void ensure_cache(idx_t idx)
{
    if (idx + 1 > cache_cap)
    {
        idx_t new_cap = (idx + 1) * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* initialise the first seven terms, if not already done   */
static void init_base_terms(void)
{
    if (cache_cap < 7)            /* need indices 0..6 */
        ensure_cache(6);

    /* if the cache is already initialised, do nothing  */
    if (last_comp != 6)
    {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)7;
        cache[6] = (seq_t)12;
        last_comp = 6;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour below offset is irrelevant       */
        return (seq_t)0;

    init_base_terms();               /* ensures initial data exists   */
    ensure_cache(n);                 /* grow cache if necessary       */

    /* compute missing terms up to n         */
    for (idx_t i = last_comp + 1; i <= n; ++i)
    {
        /* recurrence: a[n] = 2*a[n-1] - a[n-2] + 2*a[n-3] - a[n-4] - a[n-6] */
        seq_t val = cache[i - 1];
        val += cache[i - 1];         /* 2*a[i-1]            */
        val += cache[i - 3];
        val += cache[i - 3];         /* +2*a[i-3]           */
        val -= cache[i - 2];         /* -a[i-2]             */
        val -= cache[i - 4];         /* -a[i-4]             */
        val -= cache[i - 6];         /* -a[i-6]             */
        cache[i] = val;
    }

    last_comp = (last_comp > n ? last_comp : n);   /* safeguard (not strictly needed) */
    return cache[n];
}
