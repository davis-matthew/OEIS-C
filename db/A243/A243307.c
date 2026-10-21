/*
 * A243307  a(n) = 2^phi(n) + phi(n).
 * https://oeis.org/A243307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^phi(n) + phi(n)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   phi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the helper functions
 * `seq_t phi(idx_t m)` from "../utils/phi.h" and
 * `seq_t pow2(idx_t exp)` from "../utils/pow2.h". */

#include "../utils/phi.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t ph = phi(n);                /* φ(n) */
    seq_t p2 = pow2((idx_t)ph);       /* 2^φ(n) */
    return p2 + ph;                   /* 2^φ(n) + φ(n) */
}
