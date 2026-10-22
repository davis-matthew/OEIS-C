/*
 * A359708  a(n) is the greatest divisor d of 2*n such that the binary
 *          expansions of d and 2*n have no common 1-bit.
 * https://oeis.org/A359708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum divisor d of 2*n such that (d & (2*n)) == 0
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* m = 2*n, largest divisor d of m with no common 1-bit */
    seq_t m = (seq_t)n + (seq_t)n;   /* 2*n */

    seq_t best = 1;                  /* at least 1 always divides m */
    uint64_t m64 = (uint64_t)m;      /* m fits in 64‑bit for the test range */

    /* integer square root of m64 */
    uint64_t limit = (uint64_t)floor(sqrt((long double)m64));

    for (uint64_t i = 1; i <= limit; ++i) {
        if (m64 % i == 0) {
            /* divisor i */
            seq_t d1 = (seq_t)i;
            if ((d1 & m) == 0 && d1 > best) best = d1;

            /* its paired divisor */
            uint64_t j = m64 / i;
            if (j != i) {
                seq_t d2 = (seq_t)j;
                if ((d2 & m) == 0 && d2 > best) best = d2;
            }
        }
    }
    return best;
}
