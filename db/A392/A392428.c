/*
 * A392428  a(n) = Sum_{k=0..floor(2*n/3)} binomial(2*k,2*n-3*k).
 * https://oeis.org/A392428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A392428: a(n) = Sum_{k=0..floor(2*n/3)} binomial(2*k,2*n-3*k)
 *
 * Recurrence: a(n) = 4*a(n-2) + a(n-3) - 2*a(n-4) + a(n-5)
 * Initial values: a(0)=1, a(1)=0, a(2)=2, a(3)=1, a(4)=6, a(5)=7
 */

#include <stddef.h>
#include <stdint.h>

static seq_t cache[10001];   /* storage for n up to 10000 */
static idx_t cache_len = 0;  /* number of terms already computed */

/* initialise the first six terms */
static void init_cache(void)
{
    if (cache_len != 0)
        return;
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)1;
    cache[4] = (seq_t)6;
    cache[5] = (seq_t)7;
    cache_len = 6;
}

/* compute a(n) with memoisation; works for any n >= 0 (up to 10000 in static array) */
seq_t a(idx_t n)
{
    init_cache();

    /* If small enough, use the static cache. */
    if (n < 10001) {
        if (n < cache_len)
            return cache[n];

        /* Extend cache up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence: a(n) = 4*a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) */
            seq_t val = 4 * cache[i - 2];
            val += cache[i - 3];
            val -= 2 * cache[i - 4];
            val += cache[i - 5];
            cache[i] = val;
        }
        cache_len = n + 1;
        return cache[n];
    }

    /* For larger n, fall back to a small dynamic cache. */
    static seq_t *dyn = NULL;
    static idx_t dyn_cap = 0;

    if (dyn == NULL) {             /* initial allocation */
        dyn_cap = (idx_t)(n + 1);
        dyn = (seq_t *)malloc(dyn_cap * sizeof(seq_t));
        if (!dyn)                /* allocation failure */
            return (seq_t)0;
        /* copy the already known terms */
        for (size_t i = 0; i < cache_len; ++i)
            dyn[i] = cache[i];
    } else if (dyn_cap <= n) {     /* grow if needed */
        idx_t new_cap = dyn_cap * 2;
        while (new_cap <= n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(dyn, new_cap * sizeof(seq_t));
        if (!tmp)                /* allocation failure */
            return (seq_t)0;
        dyn = tmp;
        dyn_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 4 * dyn[i - 2];
        val += dyn[i - 3];
        val -= 2 * dyn[i - 4];
        val += dyn[i - 5];
        dyn[i] = val;
    }
    cache_len = n + 1;

    /* Store back into static cache if possible */
    if (n < 10001) {
        for (idx_t i = 0; i <= n; ++i)
            cache[i] = dyn[i];
    }

    return dyn[n];
}
