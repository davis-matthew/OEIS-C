/*
 * A357736  Expansion of e.g.f. sin( sqrt(2) * (exp(x) - 1) )/sqrt(2).
 * https://oeis.org/A357736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} (-2)^k * Stirling2(n, 2*k+1)
 * Range:     i8 n <= 7, i16 n <= 9, i32 n <= 15, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc/realloc */

seq_t a(idx_t n)
{
    /* Static buffers for memoisation */
    static seq_t *a_cache = NULL;   /* a(n) values          */
    static size_t a_cap = 0;        /* capacity of a_cache  */
    static seq_t *S_arr = NULL;    /* current Stirling row  */
    static size_t s_cap = 0;        /* capacity of S_arr    */
    static idx_t computed_max = 0;  /* largest n computed   */
    static int initialized = 0;    /* flag for global init */

    /* Make sure we have space for indices up to n in a_cache and S_arr */
    if (n >= (idx_t)a_cap) {
        size_t new_cap = (size_t)n + 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        /* initialise new entries to 0 (they will be filled when computed) */
        for (idx_t i = a_cap; i < new_cap; ++i) a_cache[i] = 0;
        a_cap = new_cap;
    }
    if (n >= (idx_t)s_cap) {
        size_t new_cap = (size_t)n + 1;
        S_arr = (seq_t *)realloc(S_arr, new_cap * sizeof(seq_t));
        /* initialise new entries to 0 */
        for (idx_t i = s_cap; i < new_cap; ++i) S_arr[i] = 0;
        s_cap = new_cap;
    }

    /* Global one‑time initialisation: S(0,0)=1 and a(0)=0 */
    if (!initialized) {
        S_arr[0] = 1;          /* Stirling(0,0) = 1          */
        a_cache[0] = 0;        /* a(0) = 0                    */
        computed_max = 0;
        initialized = 1;
    }

    /* Compute all values up to n that have not yet been computed. */
    for (idx_t cur = computed_max + 1; cur <= n; ++cur) {
        /* Update Stirling numbers for current row (reverse order) */
        for (idx_t m = cur; m >= 1; --m) {
            S_arr[m] = (seq_t)m * S_arr[m] + S_arr[m - 1];
        }
        S_arr[0] = 0;  /* S(n,0)=0 for n>0 */

        /* Compute a(cur) = sum_{odd m} (-2)^{(m-1)/2} * S(n,m) */
        seq_t sum = 0;
        seq_t coef = 1;  /* (-2)^0 */
        for (idx_t m = 1; m <= cur; m += 2) {
            sum += coef * S_arr[m];
            coef = -2 * coef;  /* next power of (-2) */
        }

        a_cache[cur] = sum;
    }

    computed_max = n;
    return a_cache[n];
}
