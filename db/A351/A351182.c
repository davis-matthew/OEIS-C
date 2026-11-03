/*
 * A351182  a(n) = Sum_{k=0..n} k^(2*k) * Stirling1(n,k).
 * https://oeis.org/A351182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^(2*k) * Stirling1(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

/* Compute A351182: a(n) = Σ_{k=0..n} k^(2k) * Stirling1(n,k)  */
/* Works for all seq_t (fixed-width unsigned or arbitrary precision)  */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate two temporary rows for the signed Stirling numbers. */
    seq_t *prev = mem_new(seq_t, (n + 1));
    seq_t *curr = mem_new(seq_t, (n + 1));
    if (!prev || !curr) {
        /* Allocation failed – return 0 (bad luck, not expected). */
        if (prev) mem_free(prev);
        if (curr) mem_free(curr);
        return (seq_t)0;
    }

    /* Initialize row 0: s(0,0)=1, all other entries 0. */
    for (idx_t i = 0; i <= n; ++i) {
        prev[i] = (seq_t)0;
        curr[i] = (seq_t)0;
    }
    prev[0] = (seq_t)1;

    /* Build Stirling numbers of the first kind up to n. */
    for (idx_t row = 1; row <= n; ++row) {
        curr[0] = (seq_t)0;          /* s(row,0)=0 for row>0 */
        for (idx_t k = 1; k <= row; ++k) {
            /* s(row,k) = s(row-1,k-1) - (row-1) * s(row-1,k) */
            seq_t a1 = prev[k - 1];
            seq_t a2 = prev[k];
            seq_t mult = (seq_t)(row - 1) * a2;
            curr[k] = a1 - mult;
        }
        /* Swap pointers for next iteration. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Compute the weighted sum. */
    seq_t result = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t power = ipow((seq_t)k, (seq_t)(2 * k));   /* k^(2k) */
        result = result + power * prev[k];
    }

    mem_free(prev);
    mem_free(curr);
    return result;
}
