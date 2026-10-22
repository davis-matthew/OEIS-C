/*
 * A367694  G.f. A(x) satisfies A(x) = 1 / ((1 + x) * (1 - x * A(x^4))).
 * https://oeis.org/A367694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Sum_{k=0..floor((n-1)/4)} a(k) * a(n-1-4*k)
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 1, a(1) = 0, a(2) = 1, a(3) = 0, ... (OEIS A367694) */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache of the computed values and a "done" flag for each entry. */
    static seq_t *vals   = NULL;   /* Array of values */
    static uint8_t *done = NULL;   /* 1 if vals[i] has a valid value */
    static size_t cap = 0;          /* Current capacity in number of elements */

    /* Ensure the arrays are large enough to hold index n. */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap * 2 : 16;
        while (newcap <= (size_t)n)
            newcap <<= 1;

        /* Reallocate vals and done.  Use explicit casts to keep C++ valid. */
        seq_t *tmp_vals = (seq_t *)realloc(vals, newcap * sizeof(seq_t));
        if (!tmp_vals) return (seq_t)0; /* Allocation failure – return 0 silently. */
        uint8_t *tmp_done = (uint8_t *)realloc(done, newcap * sizeof(uint8_t));
        if (!tmp_done) {
            free(tmp_vals);
            return (seq_t)0;
        }

        /* Initialise the newly added entries. */
        for (size_t i = cap; i < newcap; ++i) {
            tmp_done[i] = 0;
        }

        vals = tmp_vals;
        done = tmp_done;
        cap = newcap;
    }

    /* Compute all values up to n iteratively, skipping already computed entries. */
    for (idx_t i = 0; i <= n; ++i) {
        if (done[(size_t)i])
            continue;               /* Already computed */

        seq_t sum = 0;
        if (i >= 1) {
            idx_t maxk = (i - 1) / 4;
            for (idx_t k = 0; k <= maxk; ++k) {
                sum += vals[(size_t)k] * vals[(size_t)(i - 1 - 4 * k)];
            }
        }

        seq_t res = (i & 1) ? (sum - (seq_t)1) : (sum + (seq_t)1);

        vals[(size_t)i] = res;
        done[(size_t)i] = 1;
    }

    return vals[(size_t)n];
}
