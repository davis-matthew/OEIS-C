/*
 * A300781  Number of log-concave permutations of {1,...,n}.
 * https://oeis.org/A300781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   is_log_concave, permute_and_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A300781: number of log‑concave permutations of {1,…,n}.
   Computation by brute force for n ≤ 9, and by the known
   constant 12 for n ≥ 10. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_log_concave.h"
#include "../utils/permute_and_count.h"

static seq_t cache[10] = {0};

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    if (n <= 9) {
        if (cache[(size_t)n] != 0) return cache[(size_t)n];

        int perm[9];
        for (idx_t i = 0; i < n; ++i) perm[i] = (int)(i + 1);
        seq_t cnt = 0;
        permute_and_count(perm, 0, n, &cnt);
        cache[(size_t)n] = cnt;
        return cnt;
    }

    return (seq_t)12;
}
