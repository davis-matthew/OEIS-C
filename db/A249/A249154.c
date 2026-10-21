/*
 * A249154  (n+1) times the number of 1's in the binary expansion of n.
 * https://oeis.org/A249154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n+1) * bitcount(n)
 * Range:     u8 n <= 54, u16 n <= 6014, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   popcnt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/popcnt_u64.h"

/* Count 1-bits in a 64‑bit integer.  Uses the compiler builtin. */

/* Return the OEIS sequence value for index n. */
seq_t a(idx_t n)
{
    unsigned int bits = popcnt_u64(n);
    return (seq_t)(n + 1) * (seq_t)bits;
}
