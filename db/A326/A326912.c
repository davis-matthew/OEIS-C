/*
 * A326912  BII-numbers of pairwise intersecting set-systems with empty
 *          intersection.
 * https://oeis.org/A326912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_valid_BII
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A326912: nth integer whose BII set‑system is pairwise intersecting
   with an empty total intersection.  Brute‑force search. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_valid_BII.h"

/* Sequence A326912: a(n) = n-th smallest integer whose BII set-system is
   pairwise intersecting with empty total intersection. n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;          /* First term is 0. */

    idx_t count     = 1;                  /* Already found 0. */
    idx_t candidate = 1;                  /* Next integer to test. */

    while (count < n) {
        if (is_valid_BII(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }
    return (seq_t)candidate;              /* Unreachable, keeps compiler satisfied. */
}
