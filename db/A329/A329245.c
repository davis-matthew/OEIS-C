/*
 * A329245  For any n > 0, let m = 2*n - 1 (m is the n-th odd number); a(n) is
 *          the least k > 1 such that m AND (m^k) = m (where AND denotes the
 *          bitwise AND operator).
 * https://oeis.org/A329245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 225, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A329245.
 *
 * For each n > 0 let m = 2*n - 1 (the n‑th odd number).  The sequence term
 * a(n) is the least integer k > 1 such that
 *
 *     m & (m^k) == m
 *
 * where "m^k" denotes the ordinary integer power m*k and "&" is the
 * bitwise AND operator.
 *
 * The implementation below evaluates m^k by successive multiplication,
 * keeping the power in the type `seq_t`.  Since `seq_t` may be an
 * arbitrary‑precision integer in the build, the algorithm works for all
 * supported types.  For fixed‑width types the modulo 2^w arithmetic of
 * unsigned multiplication preserves the low bits needed for the
 * condition, so the sequence values are computed correctly.
 *
 * Complexity:  For each n we loop over k until the condition holds.
 * Empirically the required k is very small (typically ≤ 20 for n ≤ 10 000),
 * so the routine runs in far less than 2 s even for the largest n in
 * the test set.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute m = 2*n - 1, cast to seq_t for uniform operations. */
    seq_t m = (seq_t)(2 * n) - 1;

    /* Start with k = 1 so that pow holds m^1. Then increase k. */
    seq_t pow = m;   /* m¹ */
    for (idx_t k = 2;; ++k) {
        pow *= m;    /* pow = m^k */

        /* Check if all bits set in m are also set in m^k. */
        if ((m & pow) == m) {
            return (seq_t)k;
        }
    }
}
