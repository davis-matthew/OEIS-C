/*
 * A350250  Numbers k such that the k-th composition in standard order is a
 *          non-alternating permutation of an initial interval of positive
 *          integers.
 * https://oeis.org/A350250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_valid_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerates nonnegative integers k and tests whether the
   k‑th composition (in graded reverse‑lexicographic order) is a
   non‑alternating permutation of {1,…,m}.  The n‑th term a(n) is the
   n‑th such integer k, with offset 1. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_valid_k.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k = 0;
    while (1) {
        if (is_valid_k(k)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
