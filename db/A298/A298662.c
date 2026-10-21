/*
 * A298662  Number of nX3 0..1 arrays with every element equal to 0, 3, 4, 6
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 40, u64 n <= 76, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq: A298662
   Empirical recurrence used by OEIS:
        a(n) =  a(n-1) + 3*a(n-2) + a(n-3)
              - 4*a(n-4) - 6*a(n-5) - 3*a(n-6)
              + 5*a(n-7) + 7*a(n-8) - a(n-9)
              + 2*a(n-11) - 2*a(n-12)
   Initialised for n = 1 … 12.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Return the n‑th term of A298662, for any n ≥ 1.
   The sequence is memoised in a static cache.
*/
seq_t a(idx_t n)
{
    /* 1‑based cache array; index 0 is unused */
    static seq_t *cache = NULL;
    static size_t cap = 0;           /* allocated capacity (elements)        */
    static idx_t max_computed = 0;   /* highest index computed so far         */
    static int init_done = 0;        /* flag that ensures one‑time init      */

    /* Ensure capacity for indices up to n */
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Initialise the first twelve terms once */
    if (!init_done) {
        cache[1] =  (seq_t)1;
        cache[2] =  (seq_t)0;
        cache[3] =  (seq_t)3;
        cache[4] =  (seq_t)1;
        cache[5] =  (seq_t)5;
        cache[6] =  (seq_t)8;
        cache[7] =  (seq_t)7;
        cache[8] =  (seq_t)25;
        cache[9] =  (seq_t)25;
        cache[10] = (seq_t)58;
        cache[11] = (seq_t)95;
        cache[12] = (seq_t)155;
        max_computed = 12;
        init_done = 1;
    }

    /* Return already computed value if possible */
    if (n <= max_computed)
        return cache[(size_t)n];

    /* Compute any missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t v =
            cache[(size_t)i - 1]           /* a(i-1)   */
          + (seq_t)3 * cache[(size_t)i - 2] /* 3*a(i-2) */
          + cache[(size_t)i - 3]           /* a(i-3)   */
          - (seq_t)4 * cache[(size_t)i - 4] /*-4*a(i-4)*/
          - (seq_t)6 * cache[(size_t)i - 5] /*-6*a(i-5)*/
          - (seq_t)3 * cache[(size_t)i - 6] /*-3*a(i-6)*/
          + (seq_t)5 * cache[(size_t)i - 7] /*+5*a(i-7)*/
          + (seq_t)7 * cache[(size_t)i - 8] /*+7*a(i-8)*/
          - cache[(size_t)i - 9]           /*-a(i-9)  */
          + (seq_t)2 * cache[(size_t)i - 11]/*+2*a(i-11)*/
          - (seq_t)2 * cache[(size_t)i - 12];/*-2*a(i-12)*/
        cache[(size_t)i] = v;
    }
    max_computed = n;
    return cache[(size_t)n];
}
