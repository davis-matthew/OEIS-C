/*
 * A269675  Number of length-n 0..5 arrays with no repeated value differing
 *          from the previous repeated value by other than plus or minus one
 *          modulo 5+1.
 * https://oeis.org/A269675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n‑1) – 13*a(n‑2) – 35*a(n‑3), a(1)=6, a(2)=36,
 *            a(3)=210
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization.  Index 0 is unused, a[1] holds a(1). */
    static seq_t *cache = NULL;    /* dynamic array of a(n) values    */
    static idx_t  cap = 0;         /* allocated size (cache includes index 0) */
    static idx_t  computed = 0;    /* largest n that has been computed */

    if (n == 0)            /* undefined for the offset, but guard anyway */
        return (seq_t)0;

    /* Grow cache if necessary */
    if (n + 1 > cap) {
        /* Allocate exact size needed for indices up to n. */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) abort(); /* SAFETY for unlikely out-of-memory */

        cache = new_cache;
        cap   = n + 1;
    }

    /* Compute any terms that haven't yet been computed */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1)
            cache[1] = (seq_t)6;
        else if (i == 2)
            cache[2] = (seq_t)36;
        else if (i == 3)
            cache[3] = (seq_t)210;
        else {
            /* Use the recurrence relation.  The subtraction is safe
               because 9*a(n‑1) is always ≥ 13*a(n‑2)+35*a(n‑3). */
            seq_t term  = cache[i - 1] * (seq_t)9;
            term        = term - cache[i - 2] * (seq_t)13;
            term        = term - cache[i - 3] * (seq_t)35;
            cache[i]    = term;
        }
    }
    computed = (computed > n) ? computed : n;
    return cache[n];
}
