/*
 * A322233  Coefficient of x^n in Product_{k=1..n} (k + x + (n+1-k)*x^2), for
 *          n >= 0.
 * https://oeis.org/A322233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1..n} (k + x + (n+1-k)·x²)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns a(n) for n ≥ 0.  The function works with any seq_t
   (uint8_t, uint16_t, uint32_t, uint64_t, unsigned __int128,
   or an arbitrary‑precision integer class) and any idx_t that
   supports the usual unsigned integer operations. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n = 0 → empty product → coefficient of x⁰ is 1 */
    if (n == 0)
        return 1;

    /* Allocate two coefficient arrays of size n+1.  They are
       reused for each factor in the product. */
    idx_t N = n;
    size_t sz = (size_t)(N + 1);
    seq_t *coeff = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *next  = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!coeff || !next) {
        free(coeff);
        free(next);
        return 0;            /* allocation failed */
    }

    coeff[0] = 1;              /* initialise to the empty product */

    for (idx_t k = 1; k <= n; ++k) {
        /* clear the buffer for the next coefficient set */
        for (size_t i = 0; i < sz; ++i)
            next[i] = 0;

        seq_t const_term   = (seq_t)k;          /* the 'k' in k + x + … */
        seq_t quad_coeff   = (seq_t)(n + 1 - k); /* coefficient of x² */

        /* The current polynomial has degree at most 2·(k‑1) */
        idx_t dmax = 2 * (k - 1);
        if (dmax > N) dmax = N;

        for (idx_t d = 0; d <= dmax; ++d) {
            seq_t val = coeff[(size_t)d];
            if (!val) continue;

            /* constant part: multiply by k */
            next[(size_t)d] += val * const_term;

            /* linear part: add val to next[d+1] */
            if (d + 1 <= N)
                next[(size_t)(d + 1)] += val;

            /* quadratic part: add val·(n+1‑k) to next[d+2] */
            if (d + 2 <= N)
                next[(size_t)(d + 2)] += val * quad_coeff;
        }

        /* swap buffers for the next iteration */
        seq_t *tmp = coeff;
        coeff = next;
        next  = tmp;
    }

    /* The coefficient of xⁿ is now in coeff[N] */
    seq_t result = coeff[(size_t)N];

    free(coeff);
    free(next);
    return result;
}
