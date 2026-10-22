/*
 * A334966  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has weakly decreasing non-adjacent parts.
 * https://oeis.org/A334966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   valid_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A334966.
   a(n) is the nth integer k such that the k‑th composition in standard
   order (graded reverse‑lexicographic) has weakly decreasing
   non‑adjacent parts.
   Implementation: enumerate k, build its composition from the binary
   representation of k, reverse the parts, and test the
   non‑adjacent weakly decreasing condition. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/valid_k.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0;; ++k) {
        if (valid_k(k)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
