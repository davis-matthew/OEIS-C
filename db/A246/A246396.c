/*
 * A246396  Nonnegative integers k satisfying cos(k) <= 0 and cos(k+1) <= 0.
 * https://oeis.org/A246396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th (0‑based) integer k such that cos(k) <= 0 and
 *            cos(k+1) <= 0. * The function computes this by scanning integers
 *            until the n‑th qualifying k is * found. It uses the standard C
 *            math library to evaluate cosine
 * Range:     u8 n <= 87, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k = 0;

    for (;; ++k) {
        double ck = cos((double)k);
        if (ck <= 0.0) {
            double ck1 = cos((double)(k + 1));
            if (ck1 <= 0.0) {
                if (count == n)
                    return (seq_t)k;
                ++count;
            }
        }
    }
}
