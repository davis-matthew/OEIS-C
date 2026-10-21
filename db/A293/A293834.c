/*
 * A293834  Triangle whose n-th row lists the squares to be removed at the
 *          n-th iteration of the construction of the Sierpinski carpet.
 * https://oeis.org/A293834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) gives the index (with 1‑based numbering) of the square that
 *            is * removed at the n‑th step of the Sierpinski carpet
 *            construction. * * The construction: * - At step 1 the central
 *            square of the single unit square is removed. * - At step k (k≥2)
 *            every remaining square is divided into 3×3 smaller...
 * Range:     u8 n <= 33, u16 n <= 585, u32 n <= 585, u64 n <= 585,
 *            u128 n <= 585, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* External integer types are supplied by the build system. */
seq_t a(idx_t n) {
    /* Handle invalid input quietly; sequence is defined for n ≥ 1. */
    if (n == 0) return 0;

    /* 1. Find the step k.  Row k contains 8^(k-1) terms (k≥1). */
    idx_t cum   = 0;                /* cumulative terms before current row */
    idx_t pow8  = 1;                /* 8^(k-1) for current k */
    idx_t pow8n = 8;                /* 8^k for next increment */
    idx_t k     = 1;                /* current step number */

    while (cum + pow8 < n) {        /* advance to row containing n */
        cum += pow8;
        ++k;
        pow8  = pow8n;
        pow8n *= 8ULL;
    }

    /* 2. Local index within this row (0‑based). */
    idx_t l = n - cum - 1;

    /* 3. Compute powers of 3 for this step. */
    seq_t pow3k_minus1 = 1;         /* 3^(k-1) as seq_t */
    for (idx_t d = 0; d < k - 1; ++d) {
        pow3k_minus1 *= 3;
    }
    seq_t pow3k = pow3k_minus1 * 3; /* 3^k */

    /* The maximum value of a parent coordinate is 3^(k-1)-1. */
    idx_t max_parent = (idx_t)pow3k_minus1;

    /* 4. Enumerate parents in row‑major order. */
    idx_t counter = 0;
    for (idx_t i = 0; i < max_parent; ++i) {
        for (idx_t j = 0; j < max_parent; ++j) {
            /* Check that this parent was not removed in earlier steps. */
            idx_t ii = i;
            idx_t jj = j;
            bool valid = true;
            for (idx_t d = 0; d < k - 1; ++d) {
                if ((ii % 3 == 1) && (jj % 3 == 1)) {
                    valid = false;
                    break;
                }
                ii /= 3;
                jj /= 3;
            }
            if (!valid) continue;

            ++counter;
            if (counter == l + 1) {
                /* 5. Compute indices of the removed square. */
                seq_t i_removed = 3 * (seq_t)i + 1;
                seq_t j_removed = 3 * (seq_t)j + 1;
                seq_t result = pow3k * i_removed + j_removed + 1;
                return result;
            }
        }
    }

    /* 6. Should never reach here if n is valid. */
    return 0;
}
