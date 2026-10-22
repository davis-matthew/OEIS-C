/*
 * A356323  a(n) = n! * Sum_{k=1..n} sigma_3(k)/k.
 * https://oeis.org/A356323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} sigma_3(k)/k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc.h"

static seq_t *sigma3 = NULL;
static seq_t *fact   = NULL;
static seq_t *a_cache = NULL;
static idx_t  max_n  = 0;

/* Compute a(n) for any n ≥ 1, memoising intermediate tables.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, but guard against it */

    /* Expand tables if necessary */
    if (n > max_n) {
        idx_t new_max = n;
        size_t size = new_max + 1;

        /* allocate new arrays */
        sigma3 = seq_alloc(size);
        fact   = seq_alloc(size);
        a_cache = seq_alloc(size);

        /* initialise to zero */
        for (idx_t i = 0; i < size; ++i) {
            sigma3[i] = 0;
            fact[i]   = 0;
            a_cache[i] = 0;
        }

        /* compute σ₃(m) for 1 ≤ m ≤ new_max */
        for (idx_t d = 1; d <= new_max; ++d) {
            seq_t d3 = (seq_t)d * (seq_t)d * (seq_t)d;
            for (idx_t m = d; m <= new_max; m += d) {
                sigma3[m] += d3;
            }
        }

        /* compute factorials */
        fact[0] = (seq_t)1;
        for (idx_t i = 1; i <= new_max; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }

        /* compute a(i) for 1 ≤ i ≤ new_max */
        a_cache[1] = (seq_t)1;          /* 1! * σ₃(1)/1 = 1 */
        for (idx_t i = 2; i <= new_max; ++i) {
            a_cache[i] = (seq_t)i * a_cache[i - 1] + fact[i - 1] * sigma3[i];
        }

        max_n = new_max;
    }

    return a_cache[n];
}
