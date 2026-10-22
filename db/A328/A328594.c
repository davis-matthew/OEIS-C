/*
 * A328594  Numbers whose binary expansion is aperiodic.
 * https://oeis.org/A328594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS sequence A328594: Numbers whose binary expansion is aperiodic.
 *  An aperiodic binary string is one whose cyclic rotations are all distinct,
 *  equivalently its minimal period equals its length.  The routine below
 *  generates numbers in ascending order and returns the n‑th aperiodic
 *  number.  It works for any build where idx_t is a 64‑bit unsigned integer.
 */
#include <stdint.h>
#include <stdbool.h>

/* Bit length of an idx_t value (0 -> 0) */
static unsigned bitlen_idx(idx_t x) {
    return x == 0 ? 0 : 64 - __builtin_clzll(x);
}

/* Returns true iff the binary representation of x is aperiodic.
 * 0 and 1 are considered aperiodic by definition. */
static bool is_aperiodic(idx_t x) {
    if (x == 0) return true;          /* 0 is aperiodic (a(1)=0)      */
    unsigned L = bitlen_idx(x);        /* length of the binary string   */
    for (unsigned d = 1; d <= L / 2; ++d) {
        if (L % d == 0) {              /* only true divisors of L       */
            bool per = true;
            for (unsigned i = 0; i < L - d; ++i) {
                if ( ((x >> i) & 1ULL) != ((x >> (i + d)) & 1ULL) ) {
                    per = false;
                    break;
                }
            }
            if (per) return false;     /* found a nontrivial period     */
        }
    }
    return true;                      /* no nontrivial period found    */
}

seq_t a(idx_t n) {
    idx_t count = 0;                   /* number of aperiodic numbers seen */
    for (idx_t x = 0; ; ++x) {
        if (is_aperiodic(x)) {
            ++count;
            if (count == n) return (seq_t)x;
        }
    }
}
