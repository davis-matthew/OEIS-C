/*
 * A283768  Numbers k such that U(k) is odd, where U = A001950 = upper Wythoff
 *          sequence.
 * https://oeis.org/A283768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer k such that U(k) = floor(k*φ) + k is
 *            odd. * Since U(k) is odd iff floor(k/φ) is odd, we test each k
 *            until n such k * are found. φ = (1+sqrt(5))/2 is computed once
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Pre‑compute φ.  Use long double for sufficient accuracy. */
    const long double sqrt5 = sqrtl(5.0L);
    const long double phi   = (1.0L + sqrt5) / 2.0L;   /* φ */

    idx_t found = 0;
    idx_t k = 0;

    for (;;)
    {
        ++k;
        /* floor(k*φ) – truncation of the positive floating product */
        unsigned long long floor_k_phi = (unsigned long long)((long double)k * phi);

        /* U(k) = k + floor(k*φ) is odd iff floor(k*φ)+k is odd. */
        if (((k + floor_k_phi) & 1ULL) != 0) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
    }
}
