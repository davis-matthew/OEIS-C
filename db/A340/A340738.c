/*
 * A340738  Denominator of a sequence of fractions converging to e.
 * https://oeis.org/A340738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(1) = 1, a(2) = 2, a(3) = 7; for n > 3, if n even : a(n) =
 *            (n+2)*a(n-1)/2 – a(n-2) – (n‑2)*a(n-3)/2 if n odd : a(n) =
 *            2*a(n‑1) + n*a(n‑2)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 27, u128 n <= 47,
 *            big any n
 * Helpers:   muldiv2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/muldiv2.h"

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined, but keeps function safe */

    /* Static cache: indices 1..current_capacity-1 are valid. */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;

    /* Initialisation: reserve space for indices up to 3. */
    if (cap == 0) {
        cap = 4;                    /* indices 0..3; ignore 0 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;       /* allocation failure */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)7;
    }

    /* Grow cache if needed. */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp    = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values up to n. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t val;
        if ((i & 1ULL) == 0ULL) {    /* i even */
            val = muldiv2((seq_t)(i + 2), cache[i-1]);   /* (i+2)*a(i-1)/2 */
            val -= cache[i-2];                            /* – a(i-2) */
            val -= muldiv2((seq_t)(i - 2), cache[i-3]);   /* – (i-2)*a(i-3)/2 */
        } else {                    /* i odd */
            val = 2 * cache[i-1] + (seq_t)i * cache[i-2];
        }
        cache[i] = val;
    }

    return cache[n];
}
