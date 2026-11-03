/*
 * A321012  Trajectory of 596 under repeated application of the map k ->
 *          A320486(k^2).
 * https://oeis.org/A321012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 931 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A321012: Trajectory of 596 under repeated application of the map
 * k -> A320486(k^2).  The map removes every digit that appears more than once
 * in the decimal expansion of k^2.
 *
 * The sequence has period 9:
 *   a(1)=596, a(2)=3216, a(3)=103425, a(4)=197325, a(5)=897162,
 *   a(6)=652, a(7)=2510, a(8)=631, a(9)=3986
 *   a(n) = a(n-9)  (n > 9).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Apply the map k -> A320486(k^2).  The operation is performed using a 64‑bit
 * intermediate to avoid overflow for seq_t values up to 32 bits.  The result
 * is converted back to seq_t; it always fits in 32 bits.  For arbitrary‑precision
 * seq_t this also works because all intermediate values are < 2^64. */
static seq_t map_k2_removed_repeat(seq_t k)
{
    /* square using an unsigned 64‑bit intermediate */
    uint64_t prod = (uint64_t)k * (uint64_t)k;

    if (prod == 0)
        return (seq_t)0;

    /* count digit occurrences in prod */
    uint8_t cnt[10] = {0};
    uint64_t tmp = prod;
    while (tmp != 0) {
        uint8_t d = (uint8_t)(tmp % 10);
        cnt[d]++;
        tmp /= 10;
    }

    /* collect digits in reverse order for later reconstruction */
    uint8_t digits[20];                  /* enough for 20 decimal digits */
    size_t len = 0;
    tmp = prod;
    while (tmp != 0) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* rebuild number keeping only digits that appear once */
    uint64_t res = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = digits[len - 1 - i];
        if (cnt[d] == 1)
            res = res * 10 + d;
    }

    return (seq_t)res;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    /* The sequence is periodic with period 9.  Compute the cycle once. */
    static bool ready = false;
    static seq_t cycle[9];

    if (!ready) {
        cycle[0] = (seq_t)596;
        for (size_t i = 1; i < 9; ++i) {
            cycle[i] = map_k2_removed_repeat(cycle[i - 1]);
        }
        ready = true;
    }

    /* n ≥ 1 is guaranteed by the problem statement. */
    idx_t zero_based = n - (idx_t)1;
    size_t idx = (size_t)(zero_based % (idx_t)9);
    return cycle[idx];
}
