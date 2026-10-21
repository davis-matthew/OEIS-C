/*
 * A270304  Numbers that equal the sum of the squares of the digits of their
 *          own square in base 15.
 * https://oeis.org/A270304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A270304: numbers that equal the sum of the squares of the
   digits of their own square in base 15.  The sequence is finite;
   we enumerate all solutions up to 400 000 and store them in a
   static table.  This function returns the n‑th term (n ≥ 1); if n
   exceeds the number of found terms, it returns 0. */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n) {
    /* static table to memoize the sequence values */
    static seq_t *terms = NULL;
    static size_t term_count = 0;
    static size_t capacity   = 0;

    /* generate the sequence once on first call */
    if (terms == NULL) {
        const size_t LIMIT = 400000UL;          /* known upper bound */
        capacity = 16;                         /* initial allocation */
        terms = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (terms == NULL)                     /* allocation failed */
            return (seq_t)0;

        /* scan all numbers from 0 to LIMIT inclusive */
        for (idx_t x = 0; x <= LIMIT; ++x) {
            /* compute x^2 in seq_t */
            seq_t sq = (seq_t)x * (seq_t)x;

            /* sum of squares of digits of sq in base 15 */
            seq_t tmp = sq;
            seq_t sum = 0;
            while (tmp > (seq_t)0) {
                int d = (int)(tmp % (seq_t)15);  /* digit 0..14 */
                sum += (seq_t)d * (seq_t)d;
                tmp /= (seq_t)15;
            }

            if (sum == (seq_t)x) {               /* record solution */
                if (term_count >= capacity) {    /* grow the table */
                    capacity *= 2;
                    seq_t *newp = (seq_t *)realloc(terms,
                                                    capacity * sizeof(seq_t));
                    if (newp == NULL) {          /* allocation failed */
                        free(terms);
                        terms = NULL;
                        return (seq_t)0;
                    }
                    terms = newp;
                }
                terms[term_count++] = (seq_t)x;
            }
        }
    }

    /* answer the requested index */
    if (n >= 1 && (size_t)(n - 1) < term_count)
        return terms[n - 1];
    else
        return (seq_t)0;
}
