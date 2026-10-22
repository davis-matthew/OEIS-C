/*
 * A351342  G.f. A(x) satisfies: A(x) = 1 + x + x^2 + x^3 * A(x/(1 - 2*x)) /
 *          (1 - 2*x).
 * https://oeis.org/A351342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A351342
   a(0) = a(1) = a(2) = 1
   for n ≥ 3:
       a(n) = Σ_{k=0}^{n-3} binomial(n-3,k) * 2^k * a(n-3-k)            */

#include <stddef.h>
#include <stdlib.h>

/* --- static caching of computed terms ----------------------------------- */

static seq_t *a_cache  = NULL;   /* allocated array of terms          */
static idx_t  cache_cap = 0;     /* allocated capacity (elements)     */
static idx_t  cache_len = 0;     /* number of terms already computed  */

/* ensure capacity for at least 'need' indices; initialise new slots to 0 */
static void
grow_cache(idx_t need)
{
    if (need <= cache_cap)                /* already enough space   */
        return;

    /* if this is the first allocation, make sure we have at least 3 values */
    if (cache_len == 0) {
        cache_len = 3;
        if (need < 3)                    /* we need 3 for the init   */
            need = 3;
        a_cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!a_cache) exit(1);           /* fatal, can't allocate     */
        for (idx_t i = 0; i < need; ++i)
            a_cache[i] = (seq_t)0;
        a_cache[0] = a_cache[1] = a_cache[2] = (seq_t)1;
    } else {
        /* resize */
        a_cache = (seq_t *)realloc(a_cache, need * sizeof(seq_t));
        if (!a_cache) exit(1);
        for (idx_t i = cache_cap; i < need; ++i)
            a_cache[i] = (seq_t)0;
    }
    cache_cap = need;
}

/* ----------------------------------------------------------------------- */

seq_t
a(idx_t n)
{
    /* handle already known values */
    if (n < cache_len)
        return a_cache[n];

    /* make sure the array can hold up to n */
    grow_cache(n + 1);

    /* compute missing values */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t m   = i - 3;                     /* number of k's */
        seq_t binom = (seq_t)1;               /* C(m,0) */
        seq_t power2 = (seq_t)1;              /* 2^0     */

        for (idx_t k = 0; k <= m; ++k) {
            seq_t term = binom * power2;
            term = term * a_cache[i - 3 - k];
            sum += term;

            if (k < m) {                      /* prepare next binom & power */
                binom = binom * (seq_t)(m - k);
                binom = binom / (seq_t)(k + 1);
                power2 = power2 * (seq_t)2;
            }
        }
        a_cache[i] = sum;
        cache_len = i + 1;                     /* update number of computed */
    }

    return a_cache[n];
}
