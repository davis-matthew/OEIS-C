/*
 * A269340  A second sequence (see A268865) connected with negabinary
 *          expansions (cf. A039724) - see comments.
 * https://oeis.org/A269340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest L such that for all 0 <= k < L, 1 - A269027(k) =
 *            A010060(n+k); computed by direct comparison, with A269027(k) =
 *            parity of the number of 1's in the negabinary expansion of k
 * Range:     u8 n <= 681, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Validated: standard, pass, 102 terms through n = 3931 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A269340_par(uint64_t x)
{
    unsigned p = 0;
    while (x) {
        p ^= 1u;
        x &= x - 1;
    }
    return p;
}

static unsigned A269340_nb(uint64_t k)
{
    const uint64_t mask = 0xAAAAAAAAAAAAAAAAULL;
    return A269340_par((k + mask) ^ mask);
}

seq_t a(idx_t n)
{
    uint64_t k = 0;
    while (k < 100000000ULL) {
        unsigned b = 1u ^ A269340_nb(k);
        unsigned t = A269340_par((uint64_t)n + k);
        if (b != t)
            break;
        k++;
    }
    return (seq_t)k;
}
