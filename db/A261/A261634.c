/*
 * A261634  Expansion of Product_{k>=0} (1+x^(4*k+1))^3.
 * https://oeis.org/A261634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Expansion of Product_{k>=0} (1+x^(4*k+1))^3
 * Range:     u8 n <= 31, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) where a is the coefficient of x^n
 * in the product ∏_{k>=0} (1 + 3x^(4k+1) + 3x^(2*(4k+1)) + x^(3*(4k+1))) */
seq_t a(idx_t n) {
    /* Allocate coefficient arrays up to index n. */
    size_t size = (size_t)n + 1;
    seq_t *coef = (seq_t*)malloc(size * sizeof(seq_t));
    seq_t *tmp  = (seq_t*)malloc(size * sizeof(seq_t));
    if (!coef || !tmp) abort();

    /* Initialize: only the zeroth coefficient is 1. */
    for (size_t i = 0; i < size; ++i) coef[i] = 0;
    coef[0] = 1;

    /* Apply the factors (1 + 3x^v + 3x^(2v) + x^(3v)) for v = 1,5,9,… ≤ n. */
    for (idx_t v = 1; v <= n; v += 4) {
        /* Zero the temporary buffer. */
        for (size_t i = 0; i < size; ++i) tmp[i] = 0;

        for (size_t i = 0; i < size; ++i) {
            seq_t c = coef[i];
            if (c == 0) continue;

            /* 0 copies of the current weight. */
            tmp[i] += c;

            /* 1 copy: add 3 * c to position i+v */
            if (i + (size_t)v < size) tmp[i + (size_t)v] += c * (seq_t)3;

            /* 2 copies: add 3 * c to position i+2v */
            if (i + 2 * (size_t)v < size) tmp[i + 2 * (size_t)v] += c * (seq_t)3;

            /* 3 copies: add c to position i+3v */
            if (i + 3 * (size_t)v < size) tmp[i + 3 * (size_t)v] += c;
        }

        /* Copy temporary results back to the main array. */
        for (size_t i = 0; i < size; ++i) coef[i] = tmp[i];
    }

    seq_t result = coef[size - 1];
    free(coef);
    free(tmp);
    return result;
}
