/*
 * A395492  Sum of the distinct primes not dividing n that are adjacent to a
 *          divisor of n.
 * https://oeis.org/A395492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A395492: Sum of the distinct primes not dividing n that are adjacent to a divisor of n. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Helper: true if p is already stored in seen[0..cnt-1] */
static inline bool contains(uint64_t p, uint64_t *seen, size_t cnt)
{
    for (size_t i = 0; i < cnt; ++i)
        if (seen[i] == p)
            return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, so 0 is unused */

    uint64_t N     = (uint64_t)n;
    seq_t    sum   = 0;
    /* array of primes already added; 512 is comfortably larger than any
       number of distinct adjacent primes for n up to 10 000 and far beyond
       the limits of typical 64‑bit n. */
    uint64_t seen[512];
    size_t   seen_cnt = 0;

    for (uint64_t d = 1; d <= N / d; ++d) {
        if (N % d != 0) continue;
        /* process divisor d */
        for (int k = -1; k <= 1; k += 2) {   /* k = -1, +1 */
            uint64_t cand = d + (uint64_t)k;
            if (cand < 2) continue;
            if (contains(cand, seen, seen_cnt)) continue;
            if (!is_prime_big((seq_t)cand)) continue;
            if (N % cand == 0) continue;   /* does divide n */
            sum += (seq_t)cand;
            seen[seen_cnt++] = cand;
        }
        /* process the complementary divisor if distinct */
        uint64_t d2 = N / d;
        if (d2 == d) continue;
        for (int k = -1; k <= 1; k += 2) {
            uint64_t cand = d2 + (uint64_t)k;
            if (cand < 2) continue;
            if (contains(cand, seen, seen_cnt)) continue;
            if (!is_prime_big((seq_t)cand)) continue;
            if (N % cand == 0) continue;
            sum += (seq_t)cand;
            seen[seen_cnt++] = cand;
        }
    }
    return sum;
}
