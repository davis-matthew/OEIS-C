/*
 * A281557  Number of nX2 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors, with the
 *          exception of exactly two elements, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A281557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 9·a(n‑1) – 24·a(n‑2) + 9·a(n‑3) + 18·a(n‑4) + 45·a(n‑5)
                  – 41·a(n‑6) – 36·a(n‑7) – 18·a(n‑8) + 36·a(n‑9) + 12·a(n‑10)
                  – 8·a(n‑12)  for n ≥ 13
   Initial values:
      a(1)=0 , a(2)=0 , a(3)=1 , a(4)=6 , a(5)=57 , a(6)=310 ,
      a(7)=1508 , a(8)=6854 , a(9)=29487 , a(10)=122326 , a(11)=493459 ,
      a(12)=1947088
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base table for n = 1 … 12 */
    static const seq_t base[13] = {
        /* 0 */ 0,
        /* 1 */ 0,
        /* 2 */ 0,
        /* 3 */ 1,
        /* 4 */ 6,
        /* 5 */ 57,
        /* 6 */ 310,
        /* 7 */ 1508,
        /* 8 */ 6854,
        /* 9 */ 29487,
        /*10 */ 122326,
        /*11 */ 493459,
        /*12 */ 1947088
    };

    if (n <= 12)
        return base[n];

    /* Static cache for computed terms */
    static seq_t *terms = NULL;
    static idx_t  max_computed = 0;   /* highest index already computed */
    static size_t capacity    = 0;    /* allocated array size */

    /* Allocate initial cache if needed */
    if (!terms) {
        capacity = 13;                           /* store indices 0 … 12 */
        terms = (seq_t *)malloc(capacity * sizeof(seq_t));
        for (size_t i = 0; i < capacity; ++i)
            terms[i] = base[i];
        max_computed = 12;
    }

    /* Grow allocation if n exceeds current capacity */
    if ((size_t)n + 1 > capacity) {
        size_t newcap = capacity * 2;
        if (newcap < (size_t)n + 1)
            newcap = (size_t)n + 1;
        terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
        capacity = newcap;
    }

    /* Compute terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;

        /* Positive contributions */
        sum += terms[i - 1] * 9;
        sum += terms[i - 3] * 9;
        sum += terms[i - 4] * 18;
        sum += terms[i - 5] * 45;
        sum += terms[i - 9] * 36;
        sum += terms[i - 10] * 12;

        /* Negative contributions */
        sum -= terms[i - 2] * 24;
        sum -= terms[i - 6] * 41;
        sum -= terms[i - 7] * 36;
        sum -= terms[i - 8] * 18;
        sum -= terms[i - 12] * 8;

        terms[i] = sum;
    }
    max_computed = n;

    return terms[n];
}
