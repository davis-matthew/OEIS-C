/*
 * A323252  Numbers k such that the numerator and denominator of the reduced
 *          fraction Sum_{i=1..k} 1/i have no digits in common.
 * https://oeis.org/A323252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   digit_counts, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th integer k such that the reduced
   fraction Sum_{i=1..k} 1/i has numerator and denominator with
   no common decimal digits. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/digit_counts.h"

seq_t a(idx_t n) {
    /* Start with H_1 = 1/1 */
    seq_t p = 1, q = 1;          /* numerator and denominator of H_k */
    idx_t k = 2;                 /* next k to test */
    idx_t found = 0;             /* how many terms we have found */

    while (true) {
        /* Update harmonic sum: H_k = H_{k-1} + 1/k */
        seq_t kk = (seq_t)k;
        seq_t new_p = p * kk + q;
        seq_t new_q = q * kk;
        seq_t g = gcd(new_p, new_q);
        p = new_p / g;
        q = new_q / g;

        /* Check whether numerator and denominator share a digit */
        unsigned char cntp[10] = {0};
        unsigned char cntq[10] = {0};
        digit_counts(p, cntp);
        digit_counts(q, cntq);
        bool disjoint = true;
        for (int d = 0; d < 10; ++d) {
            if (cntp[d] && cntq[d]) {
                disjoint = false;
                break;
            }
        }

        if (disjoint) {
            ++found;
            if (found == n)
                return (seq_t)k;  /* found the n-th term */
        }

        ++k;
    }
}
