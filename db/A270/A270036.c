/*
 * A270036  a(n) is the smallest b for which the base-b representation of n
 *          contains a digit greater than 9.
 * https://oeis.org/A270036
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base b (b ≥ 11) such that the base‑b
 *            representation of n contains a digit ≥ 10
 * Range:     u8 n <= 1468, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (For n < 10 the behaviour is undefined, as these indices are below the OEIS offset.) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* the function is valid for n ≥ 10 only, but we return 0 for smaller inputs */
    if (n < 10u) {
        return 0;
    }

    seq_t N = (seq_t)n;
    /* search for the smallest base b >= 11 that produces a digit ≥ 10 */
    for (seq_t b = 11; b <= N + 1; ++b) {
        seq_t t = N;
        while (t > 0) {
            if (t % b >= 10) {
                return b;
            }
            t /= b;
        }
    }
    /* guaranteed to be reached only if N < 11 (i.e., n < 10) */
    return N + 1;
}
