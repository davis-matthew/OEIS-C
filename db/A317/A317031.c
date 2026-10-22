/*
 * A317031  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          4, 5, 6 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A317031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Length-15 recurrence for OEIS A317031:
   a(n) = 2*a(n-1)+4*a(n-2)+8*a(n-3)-13*a(n-4)-45*a(n-5)-55*a(n-6)
          +30*a(n-7)+163*a(n-8)+194*a(n-9)+12*a(n-10)
          -223*a(n-11)-209*a(n-12)-51*a(n-13)+37*a(n-14)+3*a(n-15)
   valid for n > 17.
   First 17 terms are supplied explicitly. */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Handle trivial n < 0 (unused). */
    if (n == (idx_t)0) return (seq_t)0;

    /* Recurrence coefficients: a(n-1) … a(n-15). */
    static const int coeff[15] = {
        2,  4,  8,   -13, -45, -55,
        30, 163, 194, 12, -223, -209,
        -51, 37,  3
    };

    /* First 17 terms, indices 1..17. */
    static const seq_t init_vals[17] = {
        (seq_t)4,       (seq_t)14,      (seq_t)30,      (seq_t)82,
        (seq_t)280,     (seq_t)842,     (seq_t)2591,    (seq_t)8141,
        (seq_t)25387,   (seq_t)79075,   (seq_t)247002,  (seq_t)771516,
        (seq_t)2408705, (seq_t)7522325, (seq_t)23494122, (seq_t)73374104,
        (seq_t)229158274
    };

    /* Static cache of computed terms, indices 0..max_computed. */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;

    /* Initialise cache on first call. */
    if (!cache) {
        /* Reserve space up to index 17 (inclusive). */
        size_t init_size = 18;            /* indices 0 .. 17 */
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) abort();
        for (int i = 1; i <= 17; ++i)
            cache[(size_t)i] = init_vals[i-1];
        max_computed = 17;
    }

    /* If requested term already known, return it. */
    if (n <= max_computed)
        return cache[(size_t)n];

    /* Ensure cache large enough for indices 0..n. */
    size_t needed = (size_t)n + 1;
    if (needed > (size_t)max_computed + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Compute missing terms. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 17) {
            /* Use the supplied initial values. */
            cache[(size_t)i] = init_vals[i-1];
        } else {
            /* Apply the 15‑term linear recurrence. */
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (int k = 0; k < 15; ++k) {
                seq_t term = cache[(size_t)(i - (idx_t)(k + 1))];
                int c = coeff[k];
                if (c >= 0)
                    sum_pos += mul_small(term, (unsigned int)c);
                else
                    sum_neg += mul_small(term, (unsigned int)(-c));
            }
            cache[(size_t)i] = sum_pos - sum_neg;
        }
    }

    max_computed = n;
    return cache[(size_t)n];
}
