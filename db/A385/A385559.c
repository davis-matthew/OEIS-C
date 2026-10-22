/*
 * A385559  Period of {binomial(N,8) mod n: N in Z}.
 * https://oeis.org/A385559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * gcd(210, n) * (4 - 3 * (n mod 2))
 * Range:     u8 n <= 9, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Convert index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    /* gcd of n and 210 */
    seq_t g = gcd(nn, (seq_t)210);
    /* parity: 1 if n odd, 0 if even */
    seq_t parity = (seq_t)(n & (idx_t)1);
    /* (4 - 3*parity) gives 1 for odd, 4 for even */
    seq_t factor = (seq_t)(4 - 3 * parity);
    return nn * g * factor;
}
