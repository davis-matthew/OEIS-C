/*
 * A387424  Partial sums of A387423.
 * https://oeis.org/A387424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 0; for n > 1, a(n) = a(n-1) + A387423(n), where
 *            A387423(k) = bitlength(k) - (length of common binary prefix of k
 *            and sigma(k)); computed by a divisor-sum sieve
 * Range:     u8 n <= 74, u16 n <= 7318, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 88 terms through n = 9904 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static unsigned A387424_bitlen(uint64_t x)
{
    unsigned r = 0;
    while (x) { ++r; x >>= 1; }
    return r;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    uint64_t *sg = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    if (!sg) return (seq_t)0;
    for (size_t d = 1; d <= N; ++d)
        for (size_t m = d; m <= N; m += d)
            sg[m] += d;

    seq_t total = (seq_t)0;
    for (size_t k = 1; k <= N; ++k) {
        uint64_t v = (uint64_t)k, s = sg[k];
        unsigned bn = A387424_bitlen(v), bs = A387424_bitlen(s);
        unsigned m = bn < bs ? bn : bs;
        uint64_t x = v >> (bn - m), y = s >> (bs - m);
        uint64_t diff = x ^ y;
        unsigned lcp = diff ? m - A387424_bitlen(diff) : m;
        total += (seq_t)(bn - lcp);
    }
    free(sg);
    return total;
}
