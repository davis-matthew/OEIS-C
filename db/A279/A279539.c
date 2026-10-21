/*
 * A279539  Sum of ceilings of natural logs of first n integers.
 * https://oeis.org/A279539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} ceil(ln(k))
 * Range:     u8 n <= 67, u16 n <= 7805, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t sum = 0;

    /* m = current ceil(ln(k))   t = exp(m)  (exactly e^m)  */
    unsigned long long m = 0;
    double t = 1.0;               /* e^0 */

    for (idx_t k = 2; k <= n; ++k) {
        /* advance m until k <= floor(e^m) */
        while ((idx_t)floor(t) < k) {
            ++m;
            t = exp((double)m);
        }
        sum += (seq_t)m;
    }
    return sum;
}
