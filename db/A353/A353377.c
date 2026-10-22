/*
 * A353377  Number of ways to write n as a product of the terms of A345452
 *          larger than 1; a(1) = 1 by convention (an empty product).
 * https://oeis.org/A353377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9658, u16 n <= 9658, u32 n <= 9658, u64 n <= 9658,
 *            u128 n <= 9658, big any n
 * Helpers:   compute_is_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A353377.  
   The value a(n) is the number of unordered factorizations of n
   into factors k > 1 that belong to sequence A345452:
      - even number of prime factors (with multiplicity)
      - even exponent of 2.
   For all n ≥ 1.  a(1) is defined to be 1 (the empty product).  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/compute_is_valid.h"

/* Global cache of whether an integer k>1 satisfies the A345452
   property (even omega and even 2-adic valuation).  */
static bool  *valid  = NULL;       /* valid[k] == true  ⇔  k ∈ A345452 */
static size_t valid_size = 0;      /* current allocation size (= highest k + 1) */

/* Ensure that 'valid' is filled for all integers up to n. */
static void
ensure_valid_up_to(idx_t n)
{
    if ((size_t)n + 1 <= valid_size) return;      /* already sufficient */

    size_t new_size   = (size_t)n + 1;             /* include index n */
    bool  *new_arr    = (bool *)realloc(valid,
                                        new_size * sizeof(bool));
    if (!new_arr) exit(1);       /* bad allocation */

    /* Fill only the new entries. */
    for (size_t i = valid_size; i < new_size; ++i) {
        new_arr[i] = compute_is_valid((idx_t)i);
    }
    valid      = new_arr;
    valid_size = new_size;
}

/* Recursive helper: count factorizations of 'rem' using only factors
   ≥ min_factor, where all factors are valid. */
static seq_t
count_factors(idx_t rem, idx_t min_factor)
{
    seq_t total = 0;

    /* Single-factor (i.e. the factor rem itself). */
    if (rem >= min_factor && valid[rem]) {
        total += (seq_t)1;
    }

    /* Try all possible first factors d in the allowed range. */
    for (idx_t d = min_factor; d * d <= rem; ++d) {
        if (rem % d != 0)          continue;     /* not a divisor */
        if (!valid[d])             continue;     /* factor must be valid */

        idx_t d2 = rem / d;        /* remaining product */
        total += count_factors(d2, d);  /* enforce non-decreasing order */
    }

    return total;
}

/* Required public function. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* empty product convention */

    ensure_valid_up_to(n);
    return count_factors(n, 2);
}
