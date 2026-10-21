/*
 * A290080  a(1) = 0; for n > 1, a(n) = sigma(bigomega(n)).
 * https://oeis.org/A290080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ(bigomega(n)), with a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    seq_t w = bigomega(n);      /* number of prime factors of n (with multiplicity) */
    seq_t sum = 0;

    for (seq_t d = 1; d*d <= w; ++d) {
        if (w % d == 0) {
            sum += d;
            seq_t e = w / d;
            if (e != d)
                sum += e;
        }
    }
    return sum;
}
