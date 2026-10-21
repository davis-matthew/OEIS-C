/*
 * A294683  Growth of the Lamplighter group: number of elements in the
 *          Lamplighter group L_2 = Z/2Z wr Z of length up to n with respect
 *          to the standard generating set {a,t}.
 * https://oeis.org/A294683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence (derived from the generating function)
   a(0)=1, a(1)=4, a(2)=10, a(3)=22, a(4)=44, a(5)=84, a(6)=155, a(7)=278
   For n >= 8
   a(n) = 1*a(n-1) + 3*a(n-2) + 0*a(n-3) + 2*a(n-6) + 3*a(n-7) + 1*a(n-8)
          - (5*a(n-4) + 3*a(n-5))
   (i.e. a(n) = pos_sum - neg_sum with pos_sum = a(n-1)+3a(n-2)+2a(n-6)+3a(n-7)+a(n-8)
                    and neg_sum = 5a(n-4)+3a(n-5).)
   The first 8 terms are stored explicitly.
   For larger n the recurrence is applied iteratively.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static const seq_t init[8] = {1, 4, 10, 22, 44, 84, 155, 278};

    /* allocate and initialise cache if needed */
    if (cache == NULL) {
        cache_size = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) return (seq_t)0; /* crash if allocation fails */
        for (idx_t i = 0; i < 8; ++i) cache[i] = init[i];
    }

    /* grow cache up to n if necessary */
    if ((idx_t)n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL) return (seq_t)0; /* crash if realloc fails */
        cache = tmp;
        /* compute new terms */
        for (idx_t i = cache_size; i <= (idx_t)n; ++i) {
            /* positive part */
            seq_t pos = cache[i-1];
            pos += (seq_t)3 * cache[i-2];
            pos += (seq_t)2 * cache[i-6];
            pos += (seq_t)3 * cache[i-7];
            pos += cache[i-8];
            /* negative part */
            seq_t neg = (seq_t)5 * cache[i-4];
            neg += (seq_t)3 * cache[i-5];
            cache[i] = pos - neg; /* pos >= neg for all n >= 8 */
        }
        cache_size = new_size;
    }

    return cache[n];
}
