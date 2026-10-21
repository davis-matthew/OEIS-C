/*
 * A289370  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 553", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A289370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   If n is odd: a(n) = 2^n - 1. If n is even: let m = n/2. Let z =
 *            0 if m ≤ 1, else z = 2^⌊log₂(m)⌋ – 1. Then a(n) = 2^n + (2^(n –
 *            z) – 1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* odd n: all 1s on the diagonal */
    if (n & 1U) {
        return big_pow((seq_t)2, (seq_t)n) - (seq_t)1;
    }

    /* even n */
    idx_t m = n >> 1;            /* m = n/2 */
    seq_t z = 0;                 /* number of consecutive zeros after the leading 1 */

    if (m >= 2) {
        unsigned long long t = 63 - __builtin_clzll(m); /* floor(log₂(m)) */
        seq_t power = ((seq_t)1) << t;                 /* 2^t */
        z = power - (seq_t)1;                          /* 2^t – 1 */
    }

    seq_t pow2n = big_pow((seq_t)2, (seq_t)n);

    /* exponent of the trailing block of 1s */
    idx_t trailing = n - (idx_t)z;                     /* n – z  (z ≤ n) */

    seq_t powTrailing = big_pow((seq_t)2, (seq_t)trailing);
    seq_t trailingOnes = powTrailing - (seq_t)1;

    return pow2n + trailingOnes;
}
