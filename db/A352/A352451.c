/*
 * A352451  2^k appears in the binary expansion of a(n) iff 2^k appears in the
 *          binary expansion of n and k+1 divides n.
 * https://oeis.org/A352451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{k≥0} 2^k * [k-th bit of n is set AND (k+1) divides n]
 * Range:     u8 n <= 260, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A352451: keep the bits of n whose positions satisfy that condition. */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t result = 0;          /* 64‑bit accumulator – safe for all bit positions */
    idx_t      k = 0;             /* current bit position   */
    idx_t      temp = n;          /* copy of n for shifting */

    while (temp) {
        if (temp & 1ULL) { /* bit k of n is set */
            if (n % (k + 1) == 0) { /* (k+1) divides n? */
                result |= 1ULL << k; /* include bit k in output */
            }
        }
        temp >>= 1ULL;          /* move to next bit */
        ++k;
    }

    return (seq_t)result;         /* cast to the required output type */
}
