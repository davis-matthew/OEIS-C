/*
 * A375414  Expansion of e.g.f. exp(-x^3 * (1 - x)) / (1 - x).
 * https://oeis.org/A375414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n * n! * Sum_{k=0..floor(n/3)} binomial(k-1,
 *            n-3*k)/k!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          which is equivalent to a(n) = n! + (-1)^n * Sum_{k=1..floor(n/3)} (n!/k!)*binomial(k-1, n-3*k) */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* factorial cache ------------------------------------------------------ */
    static seq_t *fact = NULL;
    static size_t fact_cap = 0;

    /* grow factorial cache to at least n */
    if ((size_t)n >= fact_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation error – unlikely in tests         */
        if (!fact) {
            tmp[0] = 1;
            fact_cap = 1;
        }
        for (size_t i = fact_cap; i < new_cap; ++i) {
            tmp[i] = tmp[i - 1] * (seq_t)i;
        }
        fact = tmp;
        fact_cap = new_cap;
    }

    seq_t fact_n = fact[(size_t)n];          /* n! */

    /* sum of the positive terms for k >= 1 --------------------------------- */
    seq_t sum_T = 0;
    idx_t kmax = n / 3;
    if (kmax > 0) {
        seq_t ratio = fact_n;                    /* n!/0! initially */
        for (idx_t k = 1; k <= kmax; ++k) {
            /* ratio = n!/k!  (after division by k) */
            ratio = ratio / (seq_t)k;

            idx_t m = n - 3 * k;
            if (m > (k - 1)) continue;           /* binomial is zero */

            seq_t bin_val = binomial((idx_t)(k - 1), m);
            if (!bin_val) continue;              /* skip if zero */

            seq_t term = ratio * bin_val;
            sum_T += term;
        }
    }

    /* combine with n! ------------------------------------------------------- */
    seq_t result = fact_n;
    if (n & 1) {            /* odd n: a(n) = n! - sum_T */
        result -= sum_T;
    } else {                /* even n: a(n) = n! + sum_T */
        result += sum_T;
    }

    return result;
}
