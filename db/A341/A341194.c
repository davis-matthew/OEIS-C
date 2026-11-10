/*
 * A341194  a(n) is the least k such that A340873(k) = n.
 * https://oeis.org/A341194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k such that A340873(k) = n (A340873(k) = steps of
 *            A245471 from k to 1); computed by searching k upward and caching
 *            the first k found for each step count
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static unsigned A341194_depth(uint64_t m)
{
    unsigned steps = 0;
    while (m != 1) {
        if ((m & 1) == 0) {
            m >>= 1;
        } else {
            m = (m ^ (m >> 1)) << 1;
        }
        steps++;
    }
    return steps;
}

seq_t a(idx_t n)
{
    static uint64_t best[128];
    static bool found[128];
    static uint64_t next_k = 1;

    if (n >= 128) return (seq_t)0;
    while (!found[n]) {
        unsigned d = A341194_depth(next_k);
        if (d < 128 && !found[d]) {
            found[d] = true;
            best[d] = next_k;
        }
        next_k++;
    }
    return (seq_t)best[n];
}
