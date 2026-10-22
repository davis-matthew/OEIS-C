/*
 * A396111  a(n) = n^4 mod (2*n+1).
 * https://oeis.org/A396111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod (2*n+1)
 * Range:     u8 n <= 134, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    seq_t base = (seq_t)n;          /* n as seq_t */
    seq_t mod  = base * 2 + 1;      /* 2*n+1 */
    return mod_pow(base, 4, mod);   /* compute n^4 mod (2*n+1) */
}
