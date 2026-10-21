/*
 * A246304  Numbers k such that cos(k) > cos(k+1) < cos(k+2).
 * https://oeis.org/A246304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th integer k such that cos(k) > cos(k+1) < cos(k+2)
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *terms = NULL;
    static idx_t term_cnt = 0;       /* number of terms stored */
    static idx_t next_k = 0;         /* next integer k to test */

    /* allocate initial capacity if needed */
    if (terms == NULL) {
        terms = (seq_t *)malloc(sizeof(seq_t) * 64);
    }

    /* generate terms until we have at least n of them */
    while (term_cnt < n) {
        /* evaluate cos(k) > cos(k+1) < cos(k+2) */
        long double c0 = cosl((long double)next_k);
        long double c1 = cosl((long double)(next_k + 1));
        long double c2 = cosl((long double)(next_k + 2));

        if (c0 > c1 && c1 < c2) {
            /* append current k as a term */
            if (term_cnt >= (idx_t)(-1)) {
                /* cannot allocate more terms; safety check */
                break;
            }

            /* grow cache if needed */
            if (term_cnt == 0 || term_cnt == (idx_t)(-1)) {
                /* no space, double capacity */
                size_t new_cap = 64;
                if (term_cnt > 0) new_cap = term_cnt * 2;
                terms = (seq_t *)realloc(terms, sizeof(seq_t) * new_cap);
            }

            terms[term_cnt] = (seq_t)next_k;
            term_cnt++;
        }
        next_k++;
    }

    return terms[n - 1];
}
