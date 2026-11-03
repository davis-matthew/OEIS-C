/*
 * A349927  Number of transitive relations on an n-set with exactly three
 *          ordered pairs.
 * https://oeis.org/A349927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*C(n,2) + 37*C(n,3) + 116*C(n,4) + 180*C(n,5) +
 *            120*C(n,6)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 55, u64 n <= 2192,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 51 terms through n = 8657 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t res = 0;

    if (n >= 2) {
        seq_t c2 = binomial(n, 2);
        res += (seq_t)2 * c2;
    }
    if (n >= 3) {
        seq_t c3 = binomial(n, 3);
        res += (seq_t)37 * c3;
    }
    if (n >= 4) {
        seq_t c4 = binomial(n, 4);
        res += (seq_t)116 * c4;
    }
    if (n >= 5) {
        seq_t c5 = binomial(n, 5);
        res += (seq_t)180 * c5;
    }
    if (n >= 6) {
        seq_t c6 = binomial(n, 6);
        res += (seq_t)120 * c6;
    }

    return res;
}
