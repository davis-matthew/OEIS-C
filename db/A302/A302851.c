/*
 * A302851  Permutation of nonnegative integers: a(0) = 0; for n >= 1, a(n) =
 *          A225901(1+A225901(n-1)).
 * https://oeis.org/A302851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; for n >= 1, a(n) = A225901(1+A225901(n-1)), where
 *            A225901 replaces each nonzero factorial-base digit d at position
 *            k by k+1-d
 * Range:     u8 n <= 33, u16 n <= 5913, u32 n <= 10079, u64 n <= 10079,
 *            u128 n <= 10079, big any n
 * Validated: standard, pass, 101 terms through n = 9497 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static seq_t A225901_a(seq_t n) {
    if (n == 0) return 0;
    idx_t k = 1;
    seq_t f = 1; /* k! */
    while (f <= n / (seq_t)(k + 1)) {
        f = f * (seq_t)(k + 1);
        ++k;
    }
    seq_t result = 0;
    seq_t rem = n;
    for (idx_t i = k; i >= 1; --i) {
        seq_t d = rem / f;
        rem -= d * f;
        if (d != 0) {
            result += ((seq_t)(i + 1) - d) * f;
        }
        f = f / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t t = A225901_a((seq_t)(n - 1));
    return A225901_a(t + 1);
}
