/*
 * A248028  a(n) = Sum_{k=0..n} |Stirling1(n, k)|*(n-k)! for n>=0.
 * https://oeis.org/A248028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} |Stirling1(n, k)| * (n-k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A248028: a(0)=1, a(1)=1, a(2)=2, a(3)=8, a(4)=65, ...    */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;      /* a(k) for k <= cache_max   */
    static idx_t cache_max = (idx_t)-1;  /* highest n computed so far */
    static size_t cache_size = 0;   /* actual allocated size */

    /* static buffers for the Stirling-based recurrence */
    static seq_t *T_old = NULL;  /* |s1(n-1,k)|*(n-1-k)!  */
    static seq_t *T_new = NULL;  /* |s1(n,k)|*(n-k)!      */
    static size_t T_size = 0;    /* size of T buffers   */

    /* ---------- first call ---------- */
    if (cache == NULL) {
        /* allocate space for all intended indices up to n */
        cache_size = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));

        T_size = (size_t)(n + 1);
        T_old = (seq_t *)malloc(T_size * sizeof(seq_t));
        T_new = (seq_t *)malloc(T_size * sizeof(seq_t));

        /* base case n = 0 */
        cache[0]   = (seq_t)1;      /* a(0) = 1 */
        cache_max  = 0;
        T_old[0]   = (seq_t)1;      /* |s1(0,0)|*(0-0)! = 1 */
    }

    /* ---------- extend cache if required ---------- */
    while (cache_max < n) {
        idx_t cur = cache_max + 1;          /* compute a(cur) */

        /* ensure buffers are large enough */
        if (T_size <= (size_t)(cur + 1)) {
            T_size = (size_t)(cur + 2);
            T_old = (seq_t *)realloc(T_old, T_size * sizeof(seq_t));
            T_new = (seq_t *)realloc(T_new, T_size * sizeof(seq_t));
        }
        if (cache_size <= (size_t)(cur + 1)) {
            cache_size = (size_t)(cur + 2);
            cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        }

        /* T_new[0] is always zero for cur > 0 */
        T_new[0] = (seq_t)0;
        seq_t sum = (seq_t)0;

        /* recurrence:
         * T_new[k] = T_old[k-1] + (cur-1)*(cur-k) * T_old[k]
         * for 1 <= k <= cur
         */
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t term1 = T_old[k-1];

            /* (cur-1)*(cur-k) comfortably fits in a 32‑bit unsigned int
             * for the intended n range (n <= 10000). */
            seq_t mul = (seq_t)((cur - 1) * (cur - k));

            seq_t term2 = T_old[k] * mul;
            T_new[k] = term1 + term2;

            sum += T_new[k];
        }

        /* cache the result and prepare for next iteration */
        cache[cur] = sum;
        cache_max = cur;

        /* swap old and new buffers for next step */
        seq_t *tmp = T_old;
        T_old = T_new;
        T_new = tmp;
    }

    return cache[n];
}
