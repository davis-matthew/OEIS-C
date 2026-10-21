/*
 * A292738  Numbers in which 8 outnumbers all other digits together.
 * https://oeis.org/A292738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A292738 – Numbers in which 8 outnumbers all other digits together.
   The function a(n) returns the n‑th positive integer (n ≥ 1) whose decimal
   representation contains strictly more digits “8” than any other digit
   combined.  The algorithm simply scans successive integers, counts
   digits using the utility function digit_counts (provided elsewhere),
   and collects those that satisfy the inequality.  Since the density of
   such numbers drops rapidly, searching up to about 10 000 terms requires
   only a few million candidate checks and easily meets the 2 s limit. */

#include <stdbool.h>
#include "../utils/digit_counts.h"   /* assumed available in the build environment */

seq_t a(idx_t n)
{
    /* Fast return for the very first term. */
    if (n == 1) return (seq_t)8;

    /* Count of qualifying numbers already found. */
    idx_t found = 1;                /* we already have 8 as the first term */
    idx_t candidate = 9;            /* start checking from the next integer */

    /* Temporary buffer for digit counts (digits 0–9). */
    unsigned char cnt[10];

    while (found < n) {
        seq_t val = (seq_t)candidate;      /* truncate to seq_t for digit analysis */

        /* Count digits of 'val'. */
        digit_counts(val, cnt);

        /* Total number of digits = sum of all counts. */
        unsigned int total_digits = 0;
        for (int d = 0; d < 10; ++d) total_digits += cnt[d];

        /* Check whether 8's outnumber the rest. */
        if (cnt[8] > total_digits - cnt[8]) {
            ++found;
            if (found == n) return val;    /* desired term found */
        }

        ++candidate;
    }

    /* Theoretically unreachable: all callers provide n ≥ 1. */
    return (seq_t)0;
}
