/*
 * A300518  The greatest prime factor of the squarefree part of n, or 1 if n
 *          is square.
 * https://oeis.org/A300518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 9790, u32 n <= 9790, u64 n <= 9790,
 *            u128 n <= 9790, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A300518: the greatest prime factor of the
   squarefree part of n, or 1 if n is a perfect square. */

/* seq_t and idx_t are supplied by the build environment.  seq_t is an
   unsigned integer type (8, 16, 32, 64 or 128 bits, or arbitrary
   precision).  idx_t is an unsigned 64‑bit index type. */
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    /* Result is the maximum odd‑exponent prime factor. */
    seq_t maxp = 1;

    if (m <= 1)
        return maxp;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (exp & 1U) {          /* odd exponent */
                seq_t sp = (seq_t)p;
                if (sp > maxp)
                    maxp = sp;
            }
        }
    }
    /* If m > 1, it is a prime factor with exponent 1 (odd). */
    if (m > 1) {
        seq_t sp = (seq_t)m;
        if (sp > maxp)
            maxp = sp;
    }

    return maxp;
}
