/*
 * A383436  a(1) = 1; a(n) = 2 + n * Sum_{k=1..n-1} a(k).
 * https://oeis.org/A383436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + n * Sum_{k=1}^{n-1} a(k) (a(1) = 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes a(n) for OEIS A383436.  The function maintains a static
   cache so that calls with arbitrary order are O(1) amortized time.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n=0, just return 0 */

    /* static cache */
    static seq_t *cache = NULL;         /* array of computed a(k) values */
    static size_t cap   = 0;            /* allocated capacity */
    static size_t len   = 0;            /* number of elements stored */
    static seq_t sum    = 0;            /* Sum_{k=1}^{len} a(k) */

    /* allocate or grow cache as needed */
    if (len == 0) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    while (len < (size_t)n) {
        seq_t next;
        if (len == 0) {                 /* a(1) = 1 */
            next = 1;
            sum = 1;
        } else {
            seq_t idx = (seq_t)(len + 1);/* index of the new term */
            next = 2 + idx * sum;      /* a(len+1) = 2 + idx * Sum_{k=1}^{len} a(k) */
            sum += next;               /* update cumulative sum */
        }

        /* grow cache if necessary */
        if (len >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { cache = NULL; cap = 0; len = 0; sum = 0; return 0; }
            cache = tmp;
            cap = new_cap;
        }

        cache[len] = next;
        len++;
    }

    return cache[n - 1];
}
