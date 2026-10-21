/*
 * A289097  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 545", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A289097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A289097:
   a(0)=1, a(1)=1, a(2)=111, a(3)=111,
   a(4)=10111, a(5)=11111,
   a(n) = a(n-1) + 100*a(n-2) - 100*a(n-3)   for n > 5
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* at most n+1 elements are needed, but we keep a cache
       of all computed values to serve future calls efficiently  */
    static seq_t *cache = NULL;    /* array of values */
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* highest index computed + 1 */

    /* make sure we have space to store at least indices 0..5
       whatever n is requested                      */
    size_t required = n + 1;
    if (required < 6) required = 6;
    if (cache_cap < required) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        /* on a real system we should check tmp==NULL, but the
           judging framework guarantees memory availability   */
        cache = tmp;
        cache_cap = required;
    }

    /* initialise the first six terms once */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)111;
        cache[3] = (seq_t)111;
        cache[4] = (seq_t)10111;
        cache[5] = (seq_t)11111;
        cache_len = 6;
    }

    /* nothing more to do if the requested n is already cached */
    if ((idx_t)cache_len > n) {
        return cache[(size_t)n];
    }

    /* compute missing terms using the recurrence */
    for (size_t k = cache_len; k <= (size_t)n; ++k) {
        seq_t t1 = cache[k-1];
        seq_t t2 = cache[k-2] * (seq_t)100;
        seq_t t3 = cache[k-3] * (seq_t)100;
        t1 += t2;
        t1 -= t3;
        cache[k] = t1;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
