/*
 * A381998  E.g.f. A(x) satisfies A(x) = 1 + x*exp(2*x)*A(x)^2.
 * https://oeis.org/A381998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..n} (2*k)^(n-k) * Catalan(k)/(n-k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *pwr = NULL;           /* (2*k)^(current_n-k)  */
static size_t pwr_size = 0;
static seq_t *cache = NULL;         /* cached a(n) values   */
static size_t cache_size = 0;
static seq_t *cat_arr = NULL;       /* Catalan numbers      */
static size_t cat_size = 0;

static void ensure_up_to(idx_t n)
{
    while (cache_size <= n) {
        idx_t cur = cache_size;  /* index to compute next */

        /* allocate or extend power array if needed */
        if (pwr_size <= cur) {
            seq_t *new_pwr = (seq_t *)realloc(pwr,
                                 (cur + 1) * sizeof(seq_t));
            for (idx_t i = pwr_size; i < cur + 1; ++i)
                new_pwr[i] = (seq_t)1;
            pwr = new_pwr;
            pwr_size = cur + 1;
        }

        /* allocate or extend cache array */
        if (cache_size <= cur) {
            seq_t *new_cache = (seq_t *)realloc(cache,
                                 (cur + 1) * sizeof(seq_t));
            cache = new_cache;
            cache_size = cur + 1;
        }

        /* allocate or extend catalan array */
        if (cat_size <= cur) {
            seq_t *new_cat = (seq_t *)realloc(cat_arr,
                                 (cur + 1) * sizeof(seq_t));
            cat_arr = new_cat;
            cat_size = cur + 1;
        }

        /* update powers for existing indices */
        for (idx_t k = 0; k < cur; ++k)
            pwr[k] = pwr[k] * (seq_t)(2 * k);
        pwr[cur] = (seq_t)1;   /* (2*cur)^(0) == 1 */

        /* compute Catalan number for cur */
        if (cur == 0)
            cat_arr[0] = (seq_t)1;
        else
            cat_arr[cur] = cat_arr[cur - 1] * (seq_t)(4 * cur - 2)
                           / (seq_t)(cur + 1);

        /* compute a(cur) */
        seq_t sum = (seq_t)0;
        seq_t P   = (seq_t)1;  /* product n!/(n-k)! for current k */
        for (idx_t k = 0; k <= cur; ++k) {
            if (k > 0)
                P *= (seq_t)(cur - (k - 1));          /* multiply by (n-(k-1)) */
            seq_t term = P * pwr[k] * cat_arr[k];
            sum += term;
        }
        cache[cur] = sum;
    }
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[n];
}
