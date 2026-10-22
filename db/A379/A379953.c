/*
 * A379953  Largest k >= 0 such that (n*k)^3/(n^3+k^3) is an integer.
 * https://oeis.org/A379953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Largest k >= 0 such that (n*k)^3/(n^3+k^3) is an integer
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n3 = sn * sn * sn;
    seq_t best = 0;

    for (idx_t ti = 1; ; ++ti) {
        seq_t t = (seq_t)ti;
        seq_t t3 = t * t * t;
        seq_t m  = t3 + (seq_t)1;

        /* Since t increases, once m > n3 we can stop. */
        if (m > n3)
            break;

        if (n3 % m == (seq_t)0) {
            seq_t k = sn * t;
            if (k > best)
                best = k;
        }
    }

    return best;
}
