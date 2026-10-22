/*
 * A336780  a(n) is the least number of repetitions such that the result of
 *          the repeated execution of the multiplication f <- f*n started at
 *          f=1 produces an overflow, when the multiplication is performed
 *          using 64-bit double precision floats according to the IEEE 754
 *          standard.
 * https://oeis.org/A336780
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = the minimal t ≥ 1 such that (double)n^t overflows the
 *            IEEE‑754 64‑bit floating point representation. This is computed
 *            by iteratively multiplying a double until it becomes infinite,
 *            counting the number of multiplications
 * Range:     u8 none, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Offset 2: only defined for n ≥ 2, but handle <2 trivially */
    if (n < 2) return 0;

    double f = 1.0;
    seq_t t = 0;

    /* Repeatedly multiply until overflow occurs */
    while (1) {
        f *= (double)n;
        ++t;
        if (isinf(f))
            break;
    }

    return t;
}
