/*
 * A256710  a(n) = (2*n-3)*a(n-1) - 2*a(n-2), a(0)=0, a(1)=1.
 * https://oeis.org/A256710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-3)*a(n-1) - 2*a(n-2) (Recurrence) a(0) = 0, a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache the computed values  */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;        /* current allocated capacity  */
    static size_t  computed = 0;      /* highest index computed so far */

    /* trivial cases  */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* initialise cache on first call */
    if (cache == NULL) {
        cap  = 2;                /* store a(0) and a(1) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;  /* allocation failure */
        cache[0] = 0;
        cache[1] = 1;
        computed = 1;            /* a(0) and a(1) are ready */
    }

    /* grow the cache if necessary */
    if ((idx_t)cap <= n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* realloc failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values up to n */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t multiplier = ((seq_t)2 * (seq_t)i) - (seq_t)3;
            seq_t term1 = multiplier * cache[i - 1];
            seq_t term2 = ((seq_t)2 * cache[i - 2]);
            cache[i] = term1 - term2;
        }
        computed = n;
    }

    return cache[n];
}
