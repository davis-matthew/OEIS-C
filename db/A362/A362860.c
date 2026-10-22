/*
 * A362860  Expansion of e.g.f. exp(-x) / (1 + LambertW(-3*x)).
 * https://oeis.org/A362860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1)^n * ∑_{k=0..n} (-3*k)^k * binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation using the recurrence for binomial coefficients and a
   helper pow_seq for fast exponentiation.  Only arithmetic on seq_t
   (with unsigned overflow semantics) is used; no floating point.   */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t pos = 0;      /* sum of terms with (n+k) even  */
    seq_t neg = 0;      /* sum of terms with (n+k) odd   */
    seq_t bin = 1;      /* current binomial coefficient C(n,k) */

    for (idx_t k = 0; k <= n; ++k) {
        /* compute (3*k)^k  (with 0^0 = 1) */
        seq_t term_pow;
        if (k == 0)
            term_pow = 1;
        else
            term_pow = pow_seq((seq_t)(3 * k), (int)k);

        /* combine with binomial coefficient */
        seq_t term = term_pow * bin;

        /* add to the appropriate sum according to parity of n+k */
        if (((n ^ k) & 1U) == 0U)  /* (n + k) even */
            pos += term;
        else                      /* (n + k) odd  */
            neg += term;

        /* update binomial coefficient for next k (skip after last) */
        if (k < n)
            bin = bin * (seq_t)(n - k) / (seq_t)(k + 1);
    }

    /* the result is guaranteed non‑negative, so unsigned subtraction works */
    return pos - neg;
}
