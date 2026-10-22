/*
 * A372358  a(n) = n XOR A086893(1+A000523(n)), where XOR is a bitwise-XOR,
 *          A003987.
 * https://oeis.org/A372358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n XOR A086893(1+A000523(n))
 * Range:     u8 n <= 511, u16 n <= 8585, u32 n <= 8585, u64 n <= 8585,
 *            u128 n <= 8585, big any n
 * Helpers:   pattern_of_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here A000523(n) = floor(log2(n)), so the argument of A086893 equals the
   binary length of n.  A086893(L) produces the unique L‑bit number with
   alternating 1 and 0 starting and ending with 1, i.e. 10101… or 110101… . */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pattern_of_len.h"

static inline idx_t bitlen_idx(idx_t n)
{
    /* n > 0 */
    return 64 - __builtin_clzll(n);
}

seq_t a(idx_t n)
{
    /* a(1) is defined; for safety, n>=1 is assumed. */
    idx_t L = bitlen_idx(n);           /* binary length of n */
    seq_t pat = pattern_of_len(L);     /* A086893(L) */
    return ((seq_t)n) ^ pat;
}
