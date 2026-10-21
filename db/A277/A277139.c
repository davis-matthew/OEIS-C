/*
 * A277139  Numbers k such that cos(k) < 0 and cos(k+2) < 0.
 * https://oeis.org/A277139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A277139 by brute‑force search:
   numbers k such that cos(k) < 0 and cos(k+2) < 0. */
#include <math.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    for (;; ++k) {
        double dk = (double)k;
        if (cos(dk) < 0.0 && cos(dk + 2.0) < 0.0) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
