/*
 * A389908  a(n) = min(n, A056539(n)), where A056539 reverses the bits in
 *          binary expansion of n and also complements them (0->1, 1->0) if n
 *          is even.
 * https://oeis.org/A389908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = min(n, A056539(n))
 * Range:     u8 n <= 255, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 102 terms through n = 8345 (2026-11-01)
 * Helpers:   ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ilog2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safeguard – actual sequence starts at n=1 */

    unsigned len = ilog2(n) + 1;

    seq_t rev = 0;
    for (unsigned i = 0; i < len; ++i) {
        if (n & ((idx_t)1 << i))
            rev |= ((seq_t)1 << (len - 1 - i));
    }

    seq_t mask = 0;
    for (unsigned i = 0; i < len; ++i)
        mask |= ((seq_t)1 << i);

    if ((n & 1) == 0)            /* even n → complement bits within the bit‑length */
        rev ^= mask;

    seq_t n_val = (seq_t)n;
    return rev < n_val ? rev : n_val;
}
