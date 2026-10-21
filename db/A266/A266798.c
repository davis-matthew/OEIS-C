/*
 * A266798  Least positive integer N such that n+N has the same digits as n
 *          and N together (without counting repetitions).
 * https://oeis.org/A266798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1477, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of a(n): the least positive integer N such that
 * the decimal‑digit set of n+N equals the union of the digit sets of
 * n and N (unique digits only). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"

/* --- helper: compute bitmask of decimal digits of a seq_t value --- */
static inline uint16_t digit_mask_seq(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    uint16_t m = 0;
    for (int d = 0; d < 10; ++d)
        if (cnt[d])
            m |= 1u << d;
    return m;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    seq_t seq_n = (seq_t)n;
    uint16_t mask_n = digit_mask_seq(seq_n);

    for (idx_t N = 1; ; ++N) {
        /* mask of digits of N */
        uint16_t maskN = 0;
        idx_t t = N;
        while (t) {
            maskN |= 1u << (t % 10);
            t /= 10;
        }

        uint16_t target_mask = mask_n | maskN;
        seq_t sum = seq_n + (seq_t)N;
        uint16_t sum_mask = digit_mask_seq(sum);

        if (sum_mask == target_mask)
            return (seq_t)N;
    }
}
