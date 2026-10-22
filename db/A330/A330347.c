/*
 * A330347  a(1)=1, a(2)=2; for n > 2, a(n) = (sum0 * sum1) mod n, where sum0
 *          is the sum of all previous even terms, sum1 is the sum of all
 *          previous odd terms.
 * https://oeis.org/A330347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2; for n>2, a(n) = (sum of previous even a(k) * sum
 *            of previous odd a(k)) mod n
 * Range:     u8 n <= 47, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *a_cache      = NULL;   /* cached terms (a[0] corresponds to a(1)) */
static size_t cache_cap   = 0;       /* allocated capacity of a_cache          */
static size_t cache_size  = 0;       /* number of terms currently stored       */
static seq_t  sum_even    = 0;       /* sum of even previous terms             */
static seq_t  sum_odd     = 0;       /* sum of odd  previous terms             */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined for n=0 */

    /* --- initialisation --------------------------------------------------- */
    if (cache_size == 0) {
        cache_cap = 2;                       /* space for a(1) and a(2)              */
        a_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!a_cache) return 0;              /* out of memory                        */
        a_cache[0] = 1;                      /* a(1)                                 */
        a_cache[1] = 2;                      /* a(2)                                 */
        sum_odd  = 1;                        /* 1 is odd                             */
        sum_even = 2;                        /* 2 is even                            */
        cache_size = 2;
    }

    /* --- cached value ---------------------------------------------- */
    if (n <= cache_size)
        return a_cache[n - 1];

    /* --- grow cache if needed -------------------------------------- */
    if (n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < n) new_cap = (new_cap == 0) ? 2 : new_cap << 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                  /* realloc failed                       */
        a_cache = tmp;
        cache_cap = new_cap;
    }

    /* --- compute terms up to n -------------------------------------- */
    while (cache_size < n) {
        seq_t mod_even = sum_even % (seq_t)n;
        seq_t mod_odd  = sum_odd  % (seq_t)n;
        seq_t val = (mod_even * mod_odd) % (seq_t)n;

        a_cache[cache_size++] = val;

        if ((val & 1) == 0)
            sum_even += val;
        else
            sum_odd  += val;
    }

    return a_cache[n - 1];
}
