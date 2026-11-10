/*
 * A247871  Least prime factor of A247679.
 * https://oeis.org/A247871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least prime factor of A247679(n), where A247679 lists the
 *            composite numbers == 17 (mod 18); computed by a search with
 *            trial division
 * Range:     u8 n <= 7, u16 n <= 2598, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 112 terms through n = 8262 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t A247871_lpf(seq_t v)
{
    if (v % 2 == 0) return 2;
    for (seq_t d = 3; d <= v / d; d += 2) {
        if (v % d == 0) return d;
    }
    return v;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t count = 0;
    seq_t v = 17;
    for (;;) {
        v += 18;
        seq_t p = A247871_lpf(v);
        if (p != v) {
            count++;
            if (count == n) return p;
        }
    }
}
