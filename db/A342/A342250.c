/*
 * A342250  Number of ways to write n as an ordered sum of seven powers of 2.
 * https://oeis.org/A342250
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, floor_log2_uint64, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A342250: Number of ordered sums of seven powers of 2 (offset 7) */
/* The value a(n) counts the number of 7‑tuples (x₁,…,x₇) of powers of 2
   such that x₁+…+x₇ = n.  The algorithm uses a simple dynamic programming
   method:  prev[s] = number of ways to reach sum s using the first (part-1)
   powers, then curr[s] = number of ways to reach s using part powers.  */

#include <stdlib.h>

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/floor_log2_uint64.h"

/* The sequence is only defined for n >= 7.  For n < 7 the function
 * returns 0 (the behaviour below the offset does not matter). */
seq_t a(idx_t n)
{
    if (n < 7) return 0;

    /* Determine all distinct powers of two ≤ n.  */
    idx_t max_pow = floor_log2_uint64(n);          /* ⌊log₂ n⌋ */
    size_t num_powers = (size_t)max_pow + 1;       /* count of powers */

    idx_t *powers = (idx_t *)malloc(num_powers * sizeof(idx_t));
    if (!powers) return 0;
    for (size_t k = 0; k < num_powers; ++k)
        powers[k] = ((idx_t)1 << k);

    /* Allocate two DP arrays of length n+1. */
    seq_t *prev = alloc_seq_array((size_t)n + 1);
    seq_t *curr = alloc_seq_array((size_t)n + 1);
    if (!prev || !curr) {
        free(powers);
        if (prev) free_seq_array(prev);
        if (curr) free_seq_array(curr);
        return 0;
    }

    /* Initialise: with 0 parts only sum 0 can be formed. */
    for (size_t i = 0; i <= (size_t)n; ++i) prev[i] = 0;
    prev[0] = 1;

    /* Add one power of two at a time, up to seven parts. */
    for (size_t part = 1; part <= 7; ++part) {
        for (idx_t s = 0; s <= n; ++s) {
            seq_t sum = 0;
            for (size_t p = 0; p < num_powers; ++p) {
                idx_t val = powers[p];
                if (val > s) break;
                sum += prev[s - val];
            }
            curr[s] = sum;
        }
        /* Prepare for next iteration: swap arrays. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[n];   /* result is now in the array 'prev' */

    /* Clean up. */
    free_seq_array(prev);
    free_seq_array(curr);
    free(powers);

    return result;
}
