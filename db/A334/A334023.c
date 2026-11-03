/*
 * A334023  Sum of unitary divisors of n that are larger than sqrt(n).
 * https://oeis.org/A334023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of unitary divisors d of n with d > sqrt(n)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9529 (2026-11-01)
 * Helpers:   factor_u64, ipow_i32, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities:
 *   factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
 *   seq_pow(seq_t base, idx_t exp)
 *   floor_sqrt(seq_t x)
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of bounds, return 0 */
    if (n == 1) return 0;          /* unitary divisors of 1 is {1} but 1 ≤ sqrt(1) */

    uint64_t pr[64];
    uint64_t ex[64];
    size_t  cnt;

    /* Factorise n */
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Compute floor sqrt(n) as seq_t */
    seq_t sqrt_n = floor_sqrt((seq_t)n);

    seq_t sum = 0;

    /* Enumerate all unitary divisors via subsets of primes */
    if (cnt == 0) {
        /* n is 1, handled above */
    } else {
        /* Each prime power p^e is either included or not */
        size_t total_masks = 1U << cnt;
        for (size_t mask = 0; mask < total_masks; ++mask) {
            seq_t div = 1;
            for (size_t i = 0; i < cnt; ++i) {
                if (mask & (1U << i)) {
                    /* multiply by p[i]^ex[i] */
                    div *= ipow_idx((seq_t)pr[i], (idx_t)ex[i]);
                }
            }
            if (div > sqrt_n) sum += div;
        }
    }
    return sum;
}
