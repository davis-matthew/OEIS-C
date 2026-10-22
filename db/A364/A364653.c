/*
 * A364653  Domination number of the n-Lucas cube graph.
 * https://oeis.org/A364653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   For n ≥ 5 a(n) = a(n‑1) + a(n‑3) if n mod 4 ≠ 3 a(n) = a(n‑1) +
 *            a(n‑3) + 1 if n mod 8 = 3 a(n) = a(n‑1) + a(n‑3) – 1 if n mod 8
 *            = 7 with initial values a(1)=a(2)=a(3)=1, a(4)=3
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *dom_cache = NULL;   /* stored values, 1‑based */
static idx_t  dom_max_computed = 0;  /* highest n having a value */

/* Compute a(n) */
seq_t a(idx_t n) {
    /* grow and initialize cache up to n */
    if (n == 0) return 0;  /* not used for the sequence */

    if (dom_cache == NULL || dom_max_computed < 4) {
        idx_t need = 4;
        seq_t *p = NULL;
        if (dom_cache == NULL)
            p = (seq_t *)malloc((need + 1) * sizeof(seq_t));
        else
            p = (seq_t *)realloc(dom_cache, (need + 1) * sizeof(seq_t));
        if (p == NULL) return 0;  /* allocation failed */
        dom_cache = p;
        /* initialise base case */
        dom_cache[0] = 0;
        dom_cache[1] = 1;
        dom_cache[2] = 1;
        dom_cache[3] = 1;
        dom_cache[4] = 3;
        dom_max_computed = 4;
    }

    if (dom_max_computed < n) {
        /* grow array */
        idx_t new_max = n;
        seq_t *p = (seq_t *)realloc(dom_cache, (new_max + 1) * sizeof(seq_t));
        if (p == NULL) return 0;  /* allocation failed */
        dom_cache = p;
        /* compute missing terms */
        for (idx_t i = dom_max_computed + 1; i <= new_max; ++i) {
            seq_t prev = dom_cache[i - 1];
            seq_t add  = dom_cache[i - 3];
            if ((i & 3) == 3) {           /* i mod 4 == 3 */
                if ((i & 7) == 3)         /* i mod 8 == 3 -> +1 */
                    dom_cache[i] = prev + add + 1;
                else                       /* i mod 8 == 7 -> -1 */
                    dom_cache[i] = prev + add - 1;
            } else {                      /* normal recurrence */
                dom_cache[i] = prev + add;
            }
        }
        dom_max_computed = new_max;
    }

    return dom_cache[n];
}
