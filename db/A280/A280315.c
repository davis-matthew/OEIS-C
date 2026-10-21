/*
 * A280315  Denominator of Farey fractions sorted according to increasing k,
 *          with k = numerator + denominator. Fractions with same k are sorted
 *          in order of increasing denominator.
 * https://oeis.org/A280315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A280315: denominator of the n‑th fraction a/b with
 *  k = a + b minimal and fractions sorted by k then by
 *  increasing denominator.  Only reduced fractions with
 *  0 ≤ a ≤ b are kept (0/1 is included).  This code
 *  generates terms on the fly without storing the whole
 *  sequence.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* n is 1‑based index.  n ≥ 1. */
    if (n == 0) return 0;          /* never used, safety */

    idx_t cur = 1;                  /* term index we are at */
    /* fraction 0/1 for k = 1 */
    if (n == 1) return (seq_t)1;
    cur++;                          /* now cur == 2 */

    /* iterate over k = numerator + denominator starting from 2 */
    for (idx_t k = 2;; ++k) {
        idx_t max_a = k / 2;         /* only a ≤ b so a ≤ k/2 */
        for (idx_t a = max_a; a > 0; --a) {
            idx_t b = k - a;         /* b = k - a, automatically ≥ a */
            if (gcd((seq_t)a, (seq_t)b) == (seq_t)1) {  /* reduced? */
                if (cur == n) return (seq_t)b;   /* denominator of a/b */
                ++cur;
            }
        }
    }
    /* unreachable */
    return 0;
}
