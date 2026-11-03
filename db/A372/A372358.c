/*
 * A372358  a(n) = n XOR A086893(1+A000523(n)), where XOR is a bitwise-XOR,
 *          A003987.
 * https://oeis.org/A372358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n XOR A086893(1+A000523(n))
 * Range:     u8 n <= 511, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 106 terms through n = 8585 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here A000523(n) = floor(log2(n)), so the argument of A086893 equals the
   binary length of n.  A086893(L) produces the unique L‑bit number with
   alternating 1 and 0 starting and ending with 1, i.e. 10101… or 110101… . */

#include <stdint.h>
#include <stddef.h>

/* Compute A086893(L): the unique L‑bit number with the required pattern.  */
static inline seq_t pattern_of_len(idx_t L)
{
    seq_t pat = 0;
    for (idx_t j = 0; j < L; ++j) {
        unsigned int pos = (unsigned int)(L - 1 - j);  /* bit position from LSB */
        if (L & 1) {                /* odd length -> 10101… pattern */
            if ((j & 1) == 0) {
                pat |= ((seq_t)1 << pos);
            }
        } else {                    /* even length -> 110101… pattern */
            if (j == 0 || j == 1 || ((j & 1) == 1 && j >= 2)) {
                pat |= ((seq_t)1 << pos);
            }
        }
    }
    return pat;
}

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
