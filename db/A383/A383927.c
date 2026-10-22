/*
 * A383927  Binary echo numbers: positive integers k such that the gpf(k-1) is
 *          a suffix of k when gpf(k-1) and k are written in binary.
 * https://oeis.org/A383927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 449, u32 n <= 3158, u64 n <= 3158,
 *            u128 n <= 3158, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Binary echo numbers: a(n) is the n-th odd integer k such that
   the largest prime factor of k-1 (gpf(k-1)) appears as a suffix
   of the binary representation of k.  */

/* This implementation scans odd numbers starting at 3 and counts
   those that satisfy the condition until the n-th is found.  No
   cache or advance state is kept, so each call works independently.
   The algorithm is O(n log n) and comfortably fast for n up to
   10 000.  Only gpf(uint64_t) from ../utils is used; all other
   operations are performed on uint64_t values and cast to seq_t
   when returned.  */

#include <stdint.h>
#include <limits.h>
#include "../utils/gpf.h"

/* Compute bit length of a 64‑bit integer.  */
static inline unsigned bitlen_uint64(uint64_t x)
{
    unsigned len = 0;
    while (x) { ++len; x >>= 1; }
    return len;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset is 1 */

    idx_t    found = 0;                  /* number of terms found so far */
    uint64_t k     = 3;                  /* first odd candidate > 1 */

    for (;;) {
        uint64_t g = gpf(k - 1);         /* largest prime factor of k-1 */

        /* If g == 2, its binary form ends in '10', which cannot be a
           suffix of an odd k (whose last bit is 1).  Skip such k. */
        if (g != 2) {
            unsigned len = bitlen_uint64(g);
            if (len > 0) {
                seq_t mask;
                /* seq_t may be larger than 64 bits; ensure the mask
                   covers at least 'len' low bits.                */
                if (len >= sizeof(seq_t) * CHAR_BIT) {
                    mask = ~((seq_t)0);
                } else {
                    mask = (((seq_t)1) << len) - 1;
                }
                if (((seq_t)k & mask) == (seq_t)g) {
                    ++found;
                    if (found == n) {
                        return (seq_t)k;  /* the n‑th binary echo number */
                    }
                }
            }
        }
        k += 2;  /* next odd integer */
    }
}
