/*
 * A326957  Total number of noncomposite parts in all partitions of n.
 * https://oeis.org/A326957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A037032(n)+A000070(n‑1) Computation: dynamic programming
 *            over partition sizes. For each part size i we consider all
 *            partitions of s‑i, add the new part i, and update counters
 *            accordingly. Complexity: O(n²) time, O(n) memory
 * Range:     u8 n <= 11, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Allocate zero‑initialized arrays for partition counts (p)
       and accumulated number of non‑composite parts (f). */
    seq_t *p = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *f = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!p || !f)
        abort();

    /* Base: one way to partition 0 (empty partition). */
    p[0] = 1;

    for (idx_t i = 1; i <= n; ++i) {
        int flag = (i == 1) ? 1 : (is_prime(i) ? 1 : 0);
        for (idx_t s = i; s <= n; ++s) {
            seq_t old_p = p[s - i];
            seq_t old_f = f[s - i];
            p[s] += old_p;
            f[s] += old_f + (seq_t)flag * old_p;
        }
    }

    seq_t result = f[n];
    free(p);
    free(f);
    return result;
}
