/*
 * A270120  Number of k with k^n=1 (mod n) and k^k=k (mod n); related to some
 *          groups of order n.
 * https://oeis.org/A270120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ k ∈ [1, n−1] : k^n ≡ 1 (mod n) and k^k ≡ k (mod n) }
 * Range:     u8 n <= 595, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 116 terms through n = 9572 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation checks the two conditions directly for each k. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    seq_t count = (seq_t)0;
    if (n <= 1)          /* a(1) = 0 by definition */
        return count;

    seq_t N = (seq_t)n;  /* modulo value as seq_t */

    for (idx_t k = 1; k < n; ++k) {
        seq_t K = (seq_t)k;

        /* Condition 1: k^n ≡ 1 (mod n) */
        if (powmod(K, n, N) != (seq_t)1)
            continue;

        /* Condition 2: k^k ≡ k (mod n) */
        if (powmod(K, k, N) == K)
            ++count;
    }
    return count;
}
