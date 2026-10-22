/*
 * A358257  The least significant digit of k such that 2^k, 5^k, 8^k start
 *          with the same digit.
 * https://oeis.org/A358257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least significant digit of the n-th positive integer k
 *            such that 2^k, 5^k, and 8^k all start with the same most
 *            significant digit. The computation iterates k from 0 upward,
 *            checks the condition using fractional parts of k*log10(b), and
 *            stops when the n‑th match is found
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑compute constants for logarithms. */
    static const double LOG10_2 = log10(2.0);
    static const double LOG10_8 = 3.0 * LOG10_2;
    /* Step sizes for the fractional parts. */
    const double step2 = LOG10_2;
    const double step8 = LOG10_8;

    /* Fractional parts of k*log10(2) and k*log10(8). */
    double f2 = 0.0;   /* == frac(k*log10(2)) */
    double f8 = 0.0;   /* == frac(k*log10(8)) */

    idx_t count = 0;   /* how many qualifying k's found */
    idx_t k = 0;       /* current exponent */

    while (1) {
        /* Leading digit of 2^k. */
        double pow2 = pow(10.0, f2) + 1e-12;    /* small epsilon to
                                                   avoid truncation errors */
        int ld2 = (int)pow2;

        /* Leading digit of 8^k. */
        double pow8 = pow(10.0, f8) + 1e-12;
        int ld8 = (int)pow8;

        /* Leading digit of 5^k.  Use 10 / 10^f2, special‑case when f2 == 0. */
        int ld5;
        if (f2 < 1e-15)
            ld5 = 1;
        else {
            double pow5 = 10.0 / pow(10.0, f2) + 1e-12;
            ld5 = (int)pow5;
        }

        /* Check the equality condition. */
        if (ld2 == ld5 && ld2 == ld8) {
            ++count;
            if (count == n)
                return (seq_t)(k % 10);  /* least significant digit of k */
        }

        /* Advance to next k. */
        ++k;
        f2 += step2;
        if (f2 >= 1.0)
            f2 -= 1.0;
        f8 += step8;
        if (f8 >= 1.0)
            f8 -= 1.0;
    }
}
