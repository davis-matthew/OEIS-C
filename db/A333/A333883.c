/*
 * A333883  Expansion of e.g.f. exp(Sum_{k>=0} x^(6*k + 1) / (6*k + 1)!).
 * https://oeis.org/A333883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A333883
    Recurrence: a(0) = 1
                a(n) = Σ_{k=0..⌊(n-1)/6⌋} binomial(n-1, 6k) * a(n-6k-1)   for n≥1
    This function memoises the sequence up to the maximum n requested.  It
    works for all provided seq_t/idx_t types, whether unsigned
    primitive or an arbitrary‑precision class.  The only external helper
    used is binomial(a,b) from "../utils/binomial.h".  It is safe to
    compile under C11 or C++17.  All arithmetic is performed with seq_t
    operators as required. */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache, number of computed elements, and capacity */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static size_t  next  = 0;  /* index of first uncomputed element */

    /* enlarge cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute all missing entries up to n */
    for (size_t m = next; m <= (size_t)n; ++m) {
        if (m == 0) {                /* a(0) = 1 */
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)0;
        idx_t rmax = (idx_t)(m - 1) / 6;
        for (idx_t k = 0; k <= rmax; ++k) {
            idx_t r = 6 * k;
            seq_t bin = binomial((idx_t)m - 1, r);
            seq_t prod = bin * cache[m - 1 - r];
            sum += prod;
        }
        cache[m] = sum;
    }

    next = (size_t)n + 1;
    return cache[(size_t)n];
}
