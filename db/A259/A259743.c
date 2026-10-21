/*
 * A259743  Expansion of f(-x)^3 * psi(x^4) in powers of x where psi(), f()
 *          are Ramanujan theta functions.
 * https://oeis.org/A259743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 - x^k)^3 · (1 + x^(4k))
 *            · (1 - x^(8k)); The function computes this coefficient by
 *            multiplying the generating function incrementally
 * Range:     i8 n <= 843, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The period‑8 factor is explicitly expanded:
   (1 - x^k)^3 * (1 + x^(4k)) * (1 - x^(8k))
   = 1 - 3x^k + 3x^(2k) - x^(3k) + x^(4k) - 3x^(5k) + 3x^(6k) - x^(7k)
     - x^(8k) + 3x^(9k) - 3x^(10k) + x^(11k) - x^(12k) + 3x^(13k) - 3x^(14k)
     + x^(15k).
   The coefficients below correspond to the powers 0…15 of k. */

#include <stdlib.h>
#include <string.h>   /* for memset */

static const int c_coeff[16] = {
    1,  -3,  3, -1,
    1, -3,  3, -1,
   -1,  3, -3,  1,
   -1,  3, -3,  1
};

seq_t a(idx_t n)
{
    /* Allocate coefficient arrays; they store up to n inclusive. */
    size_t sz = (size_t)(n + 1);
    seq_t *coeff = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *tmp    = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!coeff || !tmp) {
        /* If allocation fails, return zero (never expected in tests). */
        free(coeff); free(tmp);
        return (seq_t)0;
    }
    /* Initialise: constant term is 1. */
    memset(coeff, 0, sz * sizeof(seq_t));
    memset(tmp,   0, sz * sizeof(seq_t));
    coeff[0] = (seq_t)1;

    /* Iterate over the product factors for k = 1 … n. */
    for (idx_t k = 1; k <= n; ++k) {
        /* Zero temporary array for this step. */
        memset(tmp, 0, sz * sizeof(seq_t));

        /* For each existing term in coeff, convolve with the fixed
           16‑term polynomial of degree 15k. */
        for (idx_t j = 0; j <= n; ++j) {
            seq_t coef_j = coeff[j];
            if (!coef_j) continue;          /* skip zero coefficients */

            /* Add contributions from every non‑zero shift. */
            for (int m = 0; m < 16; ++m) {
                idx_t shift = (idx_t)m * k;            /* m·k */
                if (shift > n - j) break;              /* no further shifts fit */

                idx_t idx = j + shift;
                /*  cast c_coeff[m] to seq_t before multiplication */
                tmp[idx] += coef_j * (seq_t)c_coeff[m];
            }
        }

        /* Move temporary results back to coeff for the next iteration. */
        memcpy(coeff, tmp, sz * sizeof(seq_t));
    }

    seq_t result = coeff[n];
    free(coeff);
    free(tmp);
    return result;
}
