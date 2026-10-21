/*
 * A287191  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 253", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=0; For odd n ≥ 3: a(n) = (10^n - 1)/9 For
 *            even n ≥ 4: a(n) = 11 * 10^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* Base cases */
    if (n == 0 || n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;

    /* Compute 10^n only once if needed */
    if (n & 1U) { /* odd n */
        seq_t pow10n = pow10(n);
        seq_t numerator = pow10n - (seq_t)1;
        return numerator / (seq_t)9;
    } else { /* even n (n >= 4) */
        seq_t pow10_nm1 = pow10(n - 1);
        return pow10_nm1 * (seq_t)11;
    }
}
