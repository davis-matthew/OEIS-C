/*
 * A337137  Variant of A332563 - binary version of Recamán concatenation
 *          sequence.
 * https://oeis.org/A337137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 2048, u32 n <= 2048, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Helpers:   mod_pow2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A337137: for a given n, returns the least positive i
   such that the binary concatenation of n, n+2, n+3, … , n+i+1
   (skipping n+1) is divisible by n+i+1. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/mod_pow2_u64.h"

/* Count the number of binary bits needed to represent x (>0).  For x==0
   the result is 0.  Works for any unsigned seq_t type that supports
   shift and comparison. */
static inline unsigned bitlen_seq_t_local(seq_t x)
{
    unsigned l = 0;
    while (x) {
        x >>= 1;
        ++l;
    }
    return l;
}

seq_t a(idx_t n)
{
    /* search for the least i >= 1 */
    for (idx_t i = 1;; ++i) {
        /* divisor for current candidate i: D = n + i + 1 */
        seq_t D = (seq_t)n + (seq_t)i + 1;

        /* remainder of the current concatenation modulo D */
        seq_t r = 0;

        /* first term in the concatenation: n */
        seq_t term = (seq_t)n;
        r = term % D;

        /* subsequent terms: n+2, n+3, … , n+i+1 */
        for (idx_t j = 2; j <= i + 1; ++j) {
            term = (seq_t)n + (seq_t)j;
            unsigned bits = bitlen_seq_t_local(term);
            seq_t powmod = mod_pow2_u64(bits, D);
            r = (r * powmod + (term % D)) % D;
        }

        if (r == 0) return (seq_t)i;
    }
}
