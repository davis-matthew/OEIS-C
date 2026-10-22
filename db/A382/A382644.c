/*
 * A382644  Number of king permutations on n elements not beginning with the
 *          smallest element.
 * https://oeis.org/A382644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A382644
   a(n) = (n+1)*a(n-1) - (n-3)*(a(n-2)+a(n-3)) + (n-2)*a(n-4)   for n ≥ 4
   a(0)=1, a(1)=0, a(2)=0, a(3)=0
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* seed values for n < 4 */
    if (n < 4) {
        switch (n) {
        case 0: return (seq_t)1;
        case 1: case 2: case 3: return (seq_t)0;
        }
    }

    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t  capacity   = 0;
    static idx_t  last_computed = 3;  /* index of highest value computed */

    /* initialise cache on first use */
    if (!cache) {
        capacity = 16;                     /* arbitrary small start size */
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();               /* out‑of‑memory */

        /* base terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
    }

    /* enlarge cache if necessary */
    if ((idx_t)capacity <= n) {
        idx_t new_cap = capacity * 2;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* out‑of‑memory */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        seq_t term = ( (seq_t)(k + 1) * cache[k - 1] )
                   - ( (seq_t)(k - 3) * ( cache[k - 2] + cache[k - 3] ) )
                   + ( (seq_t)(k - 2) * cache[k - 4] );
        cache[k] = term;
    }
    last_computed = n;

    return cache[n];
}
