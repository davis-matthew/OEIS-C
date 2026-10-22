/*
 * A334631  a(n) = number of letters in the sum of all previous terms, with
 *          a(1) = 1 (in US English, excluding spaces and hyphens).
 * https://oeis.org/A334631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   letter_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n > 1,
   a(n) = number of letters (US English, no spaces or hyphens)
          in the sum of all previous terms:
          a(n) = letter_count(∑_{i=1}^{n-1} a(i)) */
#include <stdlib.h>
#include "../utils/letter_count.h"

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;    /* a(i) values (1‑based) */
    static seq_t *cum = NULL;    /* cumulative sums ∑_{i=1}^{k} a(i) */
    static size_t cap = 0;       /* allocated capacity (array size) */
    static size_t len = 0;       /* largest n for which values are known */

    if (n == 0)
        return 0;                /* undefined for 0, return 0 for safety */

    /* Ensure we have space for index n (1‑based). Allocate one extra slot. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        new_cap++;                      /* one more slot for 1‑based indexing */

        seq_t *new_arr = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
        seq_t *new_cum = (seq_t *)realloc(cum, new_cap * sizeof(seq_t));
        if (!new_arr || !new_cum) {
            /* Allocation failed – terminate the program. */
            exit(1);
        }
        arr = new_arr;
        cum = new_cum;
        cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (size_t i = len + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            arr[1] = (seq_t)1;
            cum[1] = arr[1];
        } else {
            seq_t sumPrev = cum[i - 1];
            seq_t ai      = (seq_t)letter_count(sumPrev);
            arr[i]        = ai;
            cum[i]        = cum[i - 1] + ai;
        }
    }
    len = (size_t)n;
    return arr[n];
}
