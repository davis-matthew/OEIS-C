/*
 * A266096  Number of n X 3 integer arrays with each element equal to the
 *          number of horizontal and antidiagonal neighbors exactly one
 *          smaller than itself.
 * https://oeis.org/A266096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 12*a(n-2) + 5*a(n-3) - 12*a(n-4) - 2*a(n-5)
 *            Initial values: a(1)=4, a(2)=13, a(3)=64, a(4)=230, a(5)=1012
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *terms = NULL;     /* 1‑based: terms[1] holds a(1) */
    static idx_t cache_n = 0;       /* largest n for which terms[n] is valid */
    static size_t capacity = 0;     /* allocated size of terms (≥ cache_n+1) */

    if (n < 1) return (seq_t)0;     /* undefined for n < 1, return 0 */

    /* Initial allocation and seed with the first five terms */
    if (terms == NULL) {
        capacity = 6;               /* indices 0..5, ignore index 0 */
        terms = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!terms) { /* allocation failed – return 0 */
            return (seq_t)0;
        }
        terms[1] = (seq_t)4;
        terms[2] = (seq_t)13;
        terms[3] = (seq_t)64;
        terms[4] = (seq_t)230;
        terms[5] = (seq_t)1012;
        cache_n = 5;
    }

    /* Grow the allocation if necessary */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(terms, sizeof(seq_t) * new_cap);
        if (!tmp) { /* realloc failed – keep old allocation but cannot compute */
            return (seq_t)0;
        }
        terms = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms using the recurrence */
    if (n > cache_n) {
        for (idx_t i = cache_n + 1; i <= n; ++i) {
            terms[i] =
                terms[i - 1]
              + (seq_t)12 * terms[i - 2]
              + (seq_t)5  * terms[i - 3]
              - (seq_t)12 * terms[i - 4]
              - (seq_t)2  * terms[i - 5];
        }
        cache_n = n;
    }

    return terms[n];
}
