/*
 * A373757  Expansion of e.g.f. exp(x^3/(6 * (1 - x)^2)).
 * https://oeis.org/A373757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/3⌋} binomial(n-k-1, n-3k) / (6^k * k!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches computed values and factorials up to the largest
   requested index.  The recurrence derived from the e.g.f. is used:
      a(0)=1, a(1)=a(2)=0
      a(n) = Σ_{j=0}^{n-3} a(j) * (n-j)*(n-j-2) * fact[n-1] / (6 * fact[j])  (n≥3)
   All divisions are exact.  The algorithm runs in O(n^2) big‑integer
   operations for each newly computed value. */

#include <stdlib.h>

static idx_t cache_size = 0;          /* size of allocated arrays (indices 0 .. cache_size-1) */
static seq_t *fact_arr = NULL;        /* factorial array: fact[i] = i!   */
static seq_t *a_arr = NULL;           /* sequence array: a_arr[i] = a(i) */

static void grow_cache(idx_t new_size) {
    if (new_size <= cache_size)
        return;

    /* Reallocate factorial and sequence arrays to the new size. */
    seq_t *tmp_fact = (seq_t *)realloc(fact_arr, new_size * sizeof(seq_t));
    seq_t *tmp_a    = (seq_t *)realloc(a_arr,    new_size * sizeof(seq_t));
    if (!tmp_fact || !tmp_a) {
        abort(); /* memory allocation failure */
    }

    fact_arr = tmp_fact;
    a_arr    = tmp_a;

    /* Compute new entries.  All indices ≥ cache_size are newly requested. */
    for (idx_t i = cache_size; i < new_size; ++i) {
        /* Factorials */
        if (i == 0) {
            fact_arr[0] = 1;
        } else {
            fact_arr[i] = fact_arr[i - 1] * i;
        }

        /* Sequence values */
        if (i == 0) {
            a_arr[0] = 1;
        } else if (i == 1 || i == 2) {
            a_arr[i] = 0;
        } else {
            /* Recurrence for i ≥ 3 */
            seq_t sum = 0;
            seq_t ratio = fact_arr[i - 1];   /* fact[i-1] / fact[0] */

            for (idx_t j = 0; j <= i - 3; ++j) {
                /* ratio currently equals fact[i-1] / fact[j] */
                seq_t term = a_arr[j] * ratio;

                idx_t ij = i - j;
                term *= ij;
                term *= (ij - 2);
                term /= 6;   /* exact division */

                sum += term;

                /* Prepare ratio for next j: divide by (j+1) */
                ratio /= (j + 1);
            }

            a_arr[i] = sum;
        }
    }

    cache_size = new_size;
}

seq_t a(idx_t n) {
    if (n >= cache_size) {
        grow_cache(n + 1);
    }
    return a_arr[n];
}
