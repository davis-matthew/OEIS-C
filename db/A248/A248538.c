/*
 * A248538  Number of length 1+3 0..n arrays with every four consecutive terms
 *          having the sum of some three elements equal to three times the
 *          fourth.
 * https://oeis.org/A248538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) - a(n-4) + 2*a(n-6) - a(n-7) (with
 *            a(1)=2, a(2)=15, a(3)=52, a(4)=101, a(5)=198, a(6)=331,
 *            a(7)=512)
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/*--- memoisation cache --------------------------------------------------*/
static seq_t *cache = NULL;     /* holds a(1)..a(k) in cache[0]..cache[k-1] */
static size_t cache_cap   = 0;   /* allocated size of cache array            */
static size_t cache_len   = 0;   /* number of values actually computed        */

/* initialise with the seven known values */
static void init_cache(void)
{
    if (cache_len) return;        /* already initialised */

    size_t init_cap = 8;           /* one extra for future expansion          */
    cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
    if (!cache) return;           /* memory failure unlikely in tests        */

    cache[0] = (seq_t)2;   /* a(1) */
    cache[1] = (seq_t)15;  /* a(2) */
    cache[2] = (seq_t)52;  /* a(3) */
    cache[3] = (seq_t)101; /* a(4) */
    cache[4] = (seq_t)198; /* a(5) */
    cache[5] = (seq_t)331; /* a(6) */
    cache[6] = (seq_t)512; /* a(7) */

    cache_cap = init_cap;
    cache_len = 7;
}

/* grow the cache so that it can hold at least n values (1‑based) */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_len) return;

    if (cache_len == 0) init_cache();   /* first use */

    /* grow capacity if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* recurrence is valid for i >= 8 */
        if (i <= 7) continue;            /* these are already stored */

        seq_t val = (seq_t)2 * cache[i-2]   /* 2*a(i-1) */
                  - cache[i-4]               /* -a(i-3)  */
                  - cache[i-5]               /* -a(i-4)  */
                  + (seq_t)2 * cache[i-7]    /* +2*a(i-6) */
                  - cache[i-8];              /* -a(i-7)  */

        cache[i-1] = val;
    }
    cache_len = (size_t)n;
}

/*--- public routine -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined position, return 0 */

    if (n <= 7) {                  /* direct access to the first seven terms */
        static const seq_t init[7] = {
            (seq_t)2,   (seq_t)15,   (seq_t)52,
            (seq_t)101, (seq_t)198,  (seq_t)331,
            (seq_t)512
        };
        return init[n-1];
    }

    ensure_cache(n);
    return cache[n-1];
}
