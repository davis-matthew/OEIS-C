/*
 * A364090  Add each term m of the sequence to the last one m times starting
 *          with 1, 1.
 * https://oeis.org/A364090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A364090: add each term m to the last value m times,
   starting with 1,1.  The sequence is generated iteratively:
   a(1)=a(2)=1;
   for i from 1 upwards:
       let m = a(i);
       append m terms, each equal to the previous term + m.
   This implementation memoises the values so that calls may come
   in any order, and each value is computed only once.
*/
#include <stdlib.h>

/* Memoised cache of the sequence */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of elements stored (>=2 after init) */
static idx_t next_i   = 1;    /* next index i to process (1‑based) */
static idx_t capacity = 0;    /* current allocated capacity */

seq_t a(idx_t n)
{
    /* a(1)=1, a(2)=1 are the first two entries */
    if (n < 1) return 0;               /* offset guard (not used by OEIS) */

    /* initialise cache on first call */
    if (cache == NULL) {
        capacity = 2;
        cache    = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return 0;   /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* generate values until we have at least n elements */
    while (cache_len < n) {
        idx_t i   = next_i;                     /* current a(i) */
        seq_t m   = cache[i-1];                 /* m = a(i) */
        seq_t last = cache[cache_len-1];         /* last existing value */

        /* append m terms, each = last + m */
        for (idx_t k = 0; k < m; ++k) {
            last = last + m;                     /* new term */
            if (cache_len == capacity) {
                capacity = capacity ? capacity << 1 : 2;
                seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
                if (tmp == NULL) {               /* out of memory */
                    free(cache);
                    cache = NULL;
                    return 0;
                }
                cache = tmp;
            }
            cache[cache_len++] = last;
        }
        /* finished this batch; move to the next i */
        ++next_i;
    }

    return cache[n-1];
}
