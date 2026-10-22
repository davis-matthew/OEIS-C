/*
 * A363447  a(0) = 0; a(n) = a(a(n-1))-1 mod (n+1) for all n >= 1.
 * https://oeis.org/A363447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0;
   for n>=1, a(n)=a(a(n-1))-1 mod (n+1) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* current size of allocated cache */

    /* initialise cache with a(0) */
    if (cap == 0) {
        cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* memory error fallback */
        cache[0] = (seq_t)0;
    }

    if (n < cap)
        return cache[n];

    /* enlarge cache to hold indices up to n */
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return (seq_t)0;          /* memory error fallback */
    cache = tmp;

    /* compute missing terms from the first missing index up to n */
    for (idx_t i = cap; i <= n; ++i) {
        seq_t prev_val = cache[i - 1];        /* a(i-1) */
        idx_t  fi      = (idx_t)prev_val;     /* index a(i-1) */
        seq_t a_prev   = cache[fi];           /* a(a(i-1)) */
        seq_t mod      = (seq_t)(i + 1);      /* modulus i+1 */

        seq_t t = a_prev % mod;              /* a_prev modulo mod */

        seq_t val;
        if (t == 0)
            val = mod - 1;                    /* -1 mod mod == mod-1 */
        else
            val = t - 1;                      /* otherwise t-1 */

        cache[i] = val;
    }

    cap = new_cap;
    return cache[n];
}
