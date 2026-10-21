/*
 * A265412  Partial sums of A265411.
 * https://oeis.org/A265412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: Let d(n) be the step added to a(n‑1). d(1) =
 *            7. For n ≥ 2, d(n) = 3 if n is in the increasing sequence 2, 3,
 *            5, 7, 10, 13, 17, 21, … defined recursively by i₁ = 2, i_{k+1} =
 *            i_k + ceil((k+1)/2), otherwise d(n) = 1. Then a(n) = a(n‑1) +
 *            d(n)
 * Range:     u8 n <= 196, u16 n <= 9958, u32 n <= 9958, u64 n <= 9958,
 *            u128 n <= 9958, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of a(0)…a(n) */
    static idx_t cap = 0;            /* current allocation size   */
    static idx_t last = 0;           /* index of last computed value */
    static idx_t next_three = 2;     /* next n for which d(n)=3   */
    static idx_t k_three = 1;        /* count of 3‑steps processed */

    /* Ensure a(0) is available */
    if (cap == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure */
        cache[0] = (seq_t)1;
        cap  = 1;
        last = 0;
    }

    /* Grow cache up to index n if necessary */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        for (idx_t i = last + 1; i < new_cap; ++i) {
            seq_t diff;
            if (i == 1) {
                diff = (seq_t)7;
            } else if (i == next_three) {
                diff = (seq_t)3;
                ++k_three;                         /* move to next 3‑index */
                idx_t inc = (k_three + 1) / 2;     /* ceil(k_three / 2) */
                next_three += inc;
            } else {
                diff = (seq_t)1;
            }
            cache[i] = cache[i - 1] + diff;
        }
        last = new_cap - 1;
        cap  = new_cap;
    }

    return cache[n];
}
