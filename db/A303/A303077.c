/*
 * A303077  a(1) = 1, and for n > 1, a(n) is the greatest prime number whose
 *          binary digits appear in order but not necessarily as consecutive
 *          digits in the binary representation of n.
 * https://oeis.org/A303077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1, for n>1 a(n)=max{ p | p prime, binary digits of p appear
 *            in the same order (not necessarily consecutive) in binary(n) }
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;

    /* get binary bits of n (least‑significant bit first) */
    int len = 0;
    idx_t tmp = n;
    while (tmp) { len++; tmp >>= 1; }

    uint8_t bits[64];
    tmp = n;
    for (int i = 0; i < len; ++i) {
        bits[i] = (uint8_t)(tmp & 1);
        tmp >>= 1;
    }

    seq_t best = 0;

    /* enumerate all non‑empty subsequences of the bit string */
    uint64_t maxMask = (uint64_t)1 << len;
    for (uint64_t mask = 1; mask < maxMask; ++mask) {
        /* find the most significant bit that is selected */
        int msb = -1;
        for (int i = len - 1; i >= 0; --i) {
            if (mask & ((uint64_t)1 << i)) { msb = i; break; }
        }
        if (msb == -1) continue;                 /* should not happen */
        if (bits[msb] == 0) continue;            /* leading zero not allowed */

        /* build the integer represented by the selected bits */
        seq_t val = 0;
        for (int i = msb; i >= 0; --i) {
            if (mask & ((uint64_t)1 << i))
                val = (val << 1) | bits[i];
        }

        if (val > best && is_prime_small(val))
            best = val;
    }

    return best;
}
