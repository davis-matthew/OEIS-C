/*
 * A352474  a(n) is the number of different ways to partition the set of
 *          vertices of a convex n-gon into 3 intersecting polygons.
 * https://oeis.org/A352474
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 12, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A352474
   Formula: a(n) = b(n) - n*(n-1)*(n-7)*(n-8)/12
   where b(8) = 0,
         b(n) = 3*b(n-1) + C(n-1,2)*(2^(n-4)+2-n-C(n-3,2))   for n>8
   Offset: 9  (a(9) is the first term listed)
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* for n < 9 the sequence is undefined; return 0 */
    if (n < 9) return 0;

    /* static state for memoisation */
    static seq_t *b_vals = NULL;   /* b[0..] */
    static seq_t *a_vals = NULL;   /* a[0..] */
    static idx_t  max_computed = 8;   /* highest n for which b and a are computed */
    static size_t capacity = 0;   /* allocated capacity of both arrays */

    /* If this is the first call or we need to grow the arrays */
    size_t needed = (size_t)n + 1;
    if (capacity < needed) {
        size_t new_cap = capacity ? capacity << 1 : 16;
        if (new_cap < needed) new_cap = needed;
        b_vals = (seq_t*)realloc(b_vals, new_cap * sizeof(seq_t));
        a_vals = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* initialise b[8] if not already done */
    if (max_computed < 8) {
        b_vals[8] = 0;
        max_computed = 8;
    }

    /* compute values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* b(i) = 3*b(i-1) + C(i-1,2)*(2^(i-4)+2-i-C(i-3,2)) */
        seq_t temp1 = 3 * b_vals[i - 1];

        /* C(i-1,2) */
        seq_t c2 = ((seq_t)(i - 1) * (seq_t)(i - 2)) / 2;

        /* C(i-3,2) */
        seq_t c3 = ((seq_t)(i - 3) * (seq_t)(i - 4)) / 2;

        /* inner term: 2^(i-4) + 2 - i - C(i-3,2) */
        seq_t inner = pow2(i - 4) + (seq_t)2 - (seq_t)i - c3;

        seq_t temp2 = c2 * inner;

        b_vals[i] = temp1 + temp2;

        /* a(i) = b(i) - i*(i-1)*(i-7)*(i-8)/12 */
        seq_t sub = ((seq_t)i * (seq_t)(i - 1) *
                     (seq_t)(i - 7) * (seq_t)(i - 8)) / 12;
        a_vals[i] = b_vals[i] - sub;
    }
    max_computed = n;

    return a_vals[n];
}
