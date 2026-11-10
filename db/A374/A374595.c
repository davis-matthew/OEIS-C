/*
 * A374595  Products of an odd number of distinct primes and the square of a
 *          number in A268390.
 * https://oeis.org/A374595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer m whose prime exponents e_1..e_k
 *            satisfy e_1 XOR ... XOR e_k = 1 (found by search with trial
 *            division)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A374595_ok(uint64_t m)
{
    uint64_t x = 0;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t c = 0;
            while (m % p == 0) { m /= p; ++c; }
            x ^= c;
        }
    }
    if (m > 1) x ^= 1;
    return x == 1;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t cand = 1;
    if (n == 0) return (seq_t)0;
    for (;;) {
        ++cand;
        if (A374595_ok(cand)) {
            ++count;
            if (count == (uint64_t)n) return (seq_t)cand;
        }
    }
}
