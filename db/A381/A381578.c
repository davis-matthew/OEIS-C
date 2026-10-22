/*
 * A381578  For n > 0, for k > n, a(n) is the least k such that the pre-period
 *          and first period of the decimal expansion of n/k contains every
 *          digit of n at least as many times it is contained in n.
 * https://oeis.org/A381578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the least k>n such that the decimal expansion of n/k *
 *            contains every digit of n at least as many times as in n, within
 *            the * pre‑period and one full period. Computation performed by
 *            exhaustive * search over k, using Floyd’s cycle detection to
 *            find the pre‑period and * period lengths, then...
 * Range:     u8 n <= 25, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    unsigned char need[10];
    digit_counts(N, need);            /* frequency of digits of n */

    for (idx_t k = n + 1; ; ++k) {
        seq_t sk = (seq_t)k;           /* denominator as seq_t */
        idx_t rem = (idx_t)(N % sk);   /* first remainder */

        /*--- Floyd’s cycle detection ------------------------------------*/
        idx_t tort = rem, hare = rem;
        do {
            tort = (idx_t)(((seq_t)tort * 10u) % sk);
            hare = (idx_t)(((seq_t)hare * 10u) % sk);
            hare = (idx_t)(((seq_t)hare * 10u) % sk);
        } while (tort != hare);

        /* pre‑period length (μ) */
        idx_t mu = 0;
        tort = rem;
        while (tort != hare) {
            tort = (idx_t)(((seq_t)tort * 10u) % sk);
            hare = (idx_t)(((seq_t)hare * 10u) % sk);
            ++mu;
        }

        /* period length (λ) */
        idx_t lambda = 1;
        hare = (idx_t)(((seq_t)hare * 10u) % sk);
        while (tort != hare) {
            hare = (idx_t)(((seq_t)hare * 10u) % sk);
            ++lambda;
        }

        /*--- Count digits in pre‑period + first period ------------------*/
        size_t total = (size_t)(mu + lambda);
        unsigned char got[10];
        memset(got, 0, sizeof(got));

        rem = (idx_t)(N % sk);          /* restart from first remainder */
        for (size_t i = 0; i < total; ++i) {
            seq_t r10 = ((seq_t)rem) * 10u;
            unsigned char d = (unsigned char)(r10 / sk);
            ++got[d];
            rem = (idx_t)(r10 % sk);
        }

        /*--- Check if all required digits are present -------------------*/
        bool ok = true;
        for (int d = 0; d < 10; ++d) {
            if (got[d] < need[d]) { ok = false; break; }
        }
        if (ok) return (seq_t)k;
    }
}
