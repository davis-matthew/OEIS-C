/*
 * A385646  a(n) is the number of distinct sums of distinct prime factors of
 *          n.
 * https://oeis.org/A385646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct subset-sums of the set of distinct *
 *            prime factors of n (excluding the empty subset)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_subset
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation note:
 *   1. Factor n by trial division up to √n, storing only distinct primes.
 *   2. Enumerate all non‑empty subsets of those primes and collect their
 *      sums into an array.
 *   3. Sort the array and count unique values.
 *   4. Return that count as a(seq_t).
 *
 * For n ≤ 10000 the number of distinct primes is ≤ 5, so the enumeration
 * is trivial. The algorithm is O(2^k) where k = ω(n), which is well
 * within the required time limits. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/cmp_subset.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* Factor n; keep only distinct primes. */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[32];
    size_t pcnt = 0;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            primes[pcnt++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) primes[pcnt++] = tmp;

    /* No prime factors? (only n = 1) */
    if (pcnt == 0) return 0;

    /* Subset enumeration. 2^pcnt subsets – empty set excluded. */
    size_t subsets = (size_t)1 << pcnt;
    seq_t sums[64];
    size_t scnt = 0;

    for (size_t mask = 1; mask < subsets; ++mask) {
        seq_t sum = 0;
        for (size_t i = 0; i < pcnt; ++i) {
            if (mask & ((size_t)1 << i))
                sum += (seq_t)primes[i];
        }
        sums[scnt++] = sum;
    }

    /* Sort sums to collapse duplicates. */
    qsort(sums, scnt, sizeof(seq_t), cmp_subset);

    /* Count distinct values. */
    seq_t count = 0;
    seq_t prev = 0;
    int first = 1;
    for (size_t i = 0; i < scnt; ++i) {
        if (first || sums[i] != prev) {
            ++count;
            prev = sums[i];
            first = 0;
        }
    }
    return count;
}
