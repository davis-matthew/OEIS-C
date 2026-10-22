/*
 * A364643  Number of separable elements of the Weyl group of type D_n.
 * https://oeis.org/A364643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*S(n) - 3*S(n-1) - 2*S(n-2) where S(k) are the large
 *            Schröder numbers: S(0) = 1, S(1) = 2 S(k) = ((6*k - 3) * S(k-1)
 *            - (k - 2) * S(k-2)) / (k + 1) for k >= 2 The first few terms are
 *            1, 2, 4, 22, 102, 474,
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *S = NULL;           /* array of large Schröder numbers */
static size_t S_cap = 0;          /* allocated capacity of S */
static idx_t S_max = 0;           /* highest index computed in S */

/* Ensure that S[0..upto] are computed and stored. */
static void _ensure_S(idx_t upto)
{
    if (S == NULL) {
        S = seq_alloc((size_t)upto + 1);
        S_cap = (size_t)upto + 1;
    } else if (S_cap < (size_t)upto + 1) {
        size_t new_cap = S_cap ? S_cap : 1;
        while (new_cap < (size_t)upto + 1) new_cap <<= 1;
        S = seq_realloc(S, S_cap, new_cap);
        S_cap = new_cap;
    }

    /* Initialise the base cases if this is the first call. */
    if (S_max == 0) {
        S[0] = (seq_t)1;
        if (S_cap > 1) S[1] = (seq_t)2;
        S_max = 1;
    }

    /* Compute further terms up to `upto`. */
    for (idx_t i = S_max + 1; i <= upto; ++i) {
        seq_t term1 = (seq_t)((6 * i) - 3) * S[(size_t)(i - 1)];
        seq_t term2 = (seq_t)(i - 2) * S[(size_t)(i - 2)];
        seq_t numerator = term1 - term2;
        seq_t denominator = (seq_t)(i + 1);
        S[(size_t)i] = numerator / denominator;
    }
    S_max = upto;
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    /* Handle the initial three terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* Compute Schröder numbers up to index n. */
    _ensure_S(n);

    /* Compute a(n) using the derived linear relation. */
    seq_t result = (seq_t)2 * S[(size_t)n]
                 - (seq_t)3 * S[(size_t)(n - 1)]
                 - (seq_t)2 * S[(size_t)(n - 2)];
    return result;
}
