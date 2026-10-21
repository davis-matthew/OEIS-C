/*
 * A250346  Number of length n arrays x(i), i=1..n with x(i) in i..i+3 and no
 *          value appearing more than 2 times.
 * https://oeis.org/A250346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 2*a(n-3) - 5*a(n-4) + a(n-6);
   initial values: a(1)=4, a(2)=16, a(3)=62, a(4)=235, a(5)=888, a(6)=3349 */
#include <stdlib.h>

/* Compute the OEIS sequence A250346.  The function is memoized: it keeps a
   static array containing all terms that have been computed so far, and it
   extends this array on the fly when a larger index is requested.  The
   recurrence uses only addition, subtraction and small integer
   multiplications, which work for all supported seq_t types, including
   arbitrary‑precision integer classes. */
seq_t a(idx_t n)
{
    /* a(0) is undefined for this sequence; return 0 for safety. */
    if (n == 0) return 0;

    /* Static cache for memoization. */
    static seq_t *cache = NULL;
    static idx_t  max_comp = 0;   /* largest n that has been computed */

    /* Allocate (or grow) the cache array to fit indices 0..n. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failure */
    } else if (n > max_comp) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      ((size_t)n + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;            /* allocation failure */
        cache = tmp;
    }

    /* Compute any missing terms up to n. */
    if (n > max_comp) {
        /* Small base terms (given by OEIS).  They are cast to seq_t
           automatically; if the value does not fit the type it will wrap
           as expected for the small‑width builds. */
        static const seq_t base[7] = { 0, 4, 16, 62, 235, 888, 3349 };

        for (idx_t i = max_comp + 1; i <= n; ++i) {
            if (i <= 6) {
                cache[i] = base[i];
            } else {
                seq_t t1 = 4 * cache[i - 1];
                seq_t t2 = 2 * cache[i - 3];
                seq_t t3 = 5 * cache[i - 4];
                cache[i] = t1 - t2 - t3 + cache[i - 6];
            }
        }
        max_comp = n;
    }

    return cache[n];
}
