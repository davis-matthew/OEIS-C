/*
 * A341869  For any k, the cumulative sum of the terms a(1) + a(2) + a(3) +
 *          ... + a(k) and the cumulative sum of their digits so far are odd.
 *          This is the lexicographically earliest sequence of distinct terms
 *          > 0 with this property.
 * https://oeis.org/A341869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A341869:  a(1)=1; for n≥2, a(n) is the (n‑1)th even integer
 * whose decimal digit sum is even (lexicographically earliest sequence of
 * distinct positive integers with odd cumulative term sum and odd cumulative
 * digit‑sum). */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    static seq_t *terms   = NULL;   /* array of computed terms          */
    static size_t capacity = 0;     /* allocated capacity              */
    static size_t length   = 0;     /* number of terms stored so far   */
    static seq_t next_candidate = 2;/* next even number to test        */

    /* Return already computed value if possible */
    if (n <= length)
        return terms[n - 1];

    /* Grow the cache until it contains a(n) */
    while (length < n) {
        seq_t x;

        /* First term is 1, all further terms are even with even digit sum */
        if (length == 0) {
            x = 1;                      /* a(1) = 1 */
        } else {
            /* Find the next even integer whose digit sum is even */
            x = next_candidate;
            while ((digit_sum(x) & 1) != 0)   /* while digit‑sum odd    */
                x += 2;                       /* keep parity even       */
            next_candidate = x + 2;           /* prepare for following  */
        }

        /* Ensure there is space in the array */
        if (length >= capacity) {
            size_t newcap = capacity ? capacity * 2 : 4;
            seq_t *newarr = (seq_t *)malloc(newcap * sizeof(seq_t));
            if (!newarr)
                abort();                       /* allocation failure    */
            if (terms) {
                memcpy(newarr, terms, length * sizeof(seq_t));
                free(terms);
            }
            terms   = newarr;
            capacity = newcap;
        }

        /* Store the new term */
        terms[length++] = x;
    }

    return terms[n - 1];
}
