/*
 * A391624  Positive integers k such that there is at most one nonnegative
 *          sequence whose first sums are the k-th composition in standard
 *          order.
 * https://oeis.org/A391624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _qualifies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391624: Positive integers k such that there is at most one
 * nonnegative sequence whose first sums are the k‑th composition in standard order.
 * The test for a given k uses the following criterion:
 *   Let s be the composition of k (as described in the OEIS entry).
 *   Define S_k recursively by S_1 = 0 and
 *          S_k = -S_{k-1} + s_{k-1}   for k ≥ 2  (s_{i} is the i‑th part of s).
 *   Let L = max ( -S_k ) over odd k,      U = min ( S_k ) over even k.
 *   Then there are at most one integer b = y_1 such that the derived sequence
 *   y satisfies y_j + y_{j+1} = s_j (j = 1 … |s|).  Thus k qualifies iff
 *   U ≤ L (zero or exactly one solution). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_qualifies.h"

#define CACHE_CAP 20000   /* sufficient for n up to about 10000 */

seq_t a(idx_t n) {
    static seq_t cache[CACHE_CAP];
    static idx_t cache_size = 0;
    static uint64_t next_k = 1;

    while (cache_size < n) {
        if (_qualifies(next_k)) {
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
