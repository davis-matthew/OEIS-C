/*
 * A381859  a(n) is the number of permutations that avoid 312 and 4321 and
 *          whose square avoids 321.
 * https://oeis.org/A381859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-4) + Sum_{j=0..n-1} a(j) initial values:
 *            a(0)=1, a(1)=1, a(2)=2
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 115,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 2889 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache of computed values and prefix sums */
    static seq_t *vals   = NULL;   /* a values  */
    static seq_t *pref   = NULL;   /* prefix sums */
    static idx_t  max_n  = 0;      /* highest n already computed */
    static size_t  cap    = 0;      /* allocated capacity */

    /* initialize cache on first call */
    if (vals == NULL) {
        /* start with a reasonable initial capacity */
        cap = 64;
        vals = mem_new(seq_t, cap);
        pref = mem_new(seq_t, cap);

        /* base cases */
        vals[0] = (seq_t)1;
        pref[0] = vals[0];
        if (cap > 1) {             /* a(1) */
            vals[1] = (seq_t)1;
            pref[1] = pref[0] + vals[1];
        }
        if (cap > 2) {             /* a(2) */
            vals[2] = (seq_t)2;
            pref[2] = pref[1] + vals[2];
        }
        max_n = 2;
    }

    /* if requested n is already computed, return it */
    if (n <= max_n)
        return vals[n];

    /* extend allocation if needed */
    if (n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        vals = mem_resize(seq_t, vals, new_cap);
        pref = mem_resize(seq_t, pref, new_cap);
        cap = new_cap;
    }

    /* compute new terms from max_n+1 up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t term = (seq_t)0;
        if (i >= 3) term += vals[i - 3];
        if (i >= 4) term += vals[i - 4];
        term += pref[i - 1];          /* sum_{j=0..i-1} a(j) */
        vals[i] = term;
        pref[i] = pref[i - 1] + term;
    }
    max_n = n;
    return vals[n];
}
