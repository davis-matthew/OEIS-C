/*
 * A259869  a(0) = -1; for n > 0, number of indecomposable derangements, i.e.,
 *          no fixed points, and not fixing [1..j] for any 1 <= j < n.
 * https://oeis.org/A259869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = -1; a(1) = 0; a(n) = (n-1)*a(n-1) + (n-3)*a(n-2) +
 *            Σ_{j=1}^{n-1} a(j)*a(n-j), n > 1 (OEIS A259869)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cache for computed values. */
static seq_t *A          = NULL;   /* sequence values */
static bool  *computed   = NULL;   /* flags of validity */
static size_t cap        = 0;      /* number of slots allocated */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cap(idx_t n)
{
    if (n + 1 <= cap) return;

    size_t new_cap = (size_t)(n + 1) * 2;
    seq_t *new_A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    bool  *new_cmp = (bool  *)realloc(computed, new_cap * sizeof(bool));
    if (!new_A || !new_cmp) abort();  /* allocation failure */

    /* Initialise new slots. */
    for (size_t i = cap; i < new_cap; ++i) {
        new_A[i] = 0;
        new_cmp[i] = false;
    }

    A = new_A;
    computed = new_cmp;
    cap = new_cap;
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    ensure_cap(n);

    /* Compute any unset values up to n. */
    for (idx_t i = 0; i <= n; ++i) {
        if (computed[i]) continue;          /* already known */

        if (i == 0) {
            A[i] = (seq_t)-1;
        } else if (i == 1) {
            A[i] = (seq_t)0;
        } else {
            /* Coefficients may be negative for small i, so cast to a signed
               type before subtraction. */
            seq_t coeff1 = (seq_t)((int64_t)i - 1);
            seq_t coeff2 = (seq_t)((int64_t)i - 3);

            seq_t term1 = coeff1 * A[i - 1];
            seq_t term2 = coeff2 * A[i - 2];

            seq_t sum = 0;
            for (idx_t j = 1; j < i; ++j) {
                sum = sum + A[j] * A[i - j];
            }

            A[i] = term1 + term2 + sum;
        }

        computed[i] = true;
    }

    return A[n];
}
