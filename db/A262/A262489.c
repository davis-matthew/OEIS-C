/*
 * A262489  The index of the first of two consecutive positive triangular
 *          numbers (A000217) the sum of which is equal to the sum of three
 *          consecutive positive triangular numbers.
 * https://oeis.org/A262489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+10*a(n-2)-10*a(n-3)-a(n-4)+a(n-5) for n>5
   a(1)=7, a(2)=18, a(3)=78, a(4)=187, a(5)=781 */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[i-1] == a(i) for i>=1 */
    static size_t cap = 0;        /* allocated size of cache in elements */
    static idx_t max_comp = 0;    /* largest n already computed */

    if (n < 1) return 0;          /* offset behaviour irrelevant */

    /* Grow cache if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        /* realloc must be cast to seq_t* for C++ compilers */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute all terms up to n that have not yet been computed */
    if (max_comp < n) {
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)  val = 7;
            else if (i == 2)  val = 18;
            else if (i == 3)  val = 78;
            else if (i == 4)  val = 187;
            else if (i == 5)  val = 781;
            else              /* recurrence for i > 5 */
                val = cache[i - 2]                 /* a(i-1) */
                    + 10 * cache[i - 3]           /* +10*a(i-2) */
                    - 10 * cache[i - 4]           /* -10*a(i-3) */
                    - cache[i - 5]                /* -a(i-4) */
                    + cache[i - 6];               /* +a(i-5) */
            cache[i - 1] = val;
        }
        max_comp = n;
    }

    return cache[n - 1];
}
