/*
 * A304399  G.f. A(x) satisfies: [x^n] (1+x)^((n+1)^4) / A(x) = 0 for n>0.
 * https://oeis.org/A304399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1. For n > 0 the auxiliary sequence B satisfies
 *
 *   B[0] = 1,
 *   B[n] = - Σ_{k=1..n}  binom((n+1)^4, k) * B[n-k].
 *
 * The original sequence is then
 *
 *   A[0] = 1,
 *   A[n] = - Σ_{k=1..n}  A[n-k] * B[k].
 *
 * This implementation memoises A and B in two static vectors and
 * returns the requested term.
 */

#include <stdlib.h>

static seq_t *A = NULL;          /* A[0] = a(0), … */
static seq_t *B = NULL;          /* B[0] = 1, B[n] = b_n */
static idx_t cap = 0;            /* allocated capacity */
static idx_t sz  = 0;            /* number of elements stored (≥0) */

/* Ensure capacity for at least 'need' elements. */
static void ensure_cap(idx_t need)
{
    if (need <= cap) return;
    idx_t newcap = cap ? (cap << 1) : 8;
    while (newcap < need) newcap <<= 1;
    A = (seq_t*)realloc(A, newcap * sizeof(seq_t));
    B = (seq_t*)realloc(B, newcap * sizeof(seq_t));
    for (idx_t i = cap; i < newcap; ++i) {
        A[i] = 0;
        B[i] = 0;
    }
    cap = newcap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Resize storage if necessary. */
    ensure_cap(n + 1);

    /* Initialise the first term if not yet done. */
    if (sz == 0) {
        A[0] = 1;
        B[0] = 1;
        sz   = 1;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = sz; i <= n; ++i) {
        /* Compute B[i] for i > 0. */
        if (i > 0) {
            seq_t k = (seq_t)(i + 1);
            k = k * k * k * k;          /* (i+1)^4 */

            seq_t bin = 1;              /* binom(k, 0) */
            seq_t sum = 0;
            for (idx_t j = 1; j <= i; ++j) {
                /* advance to binom(k, j) */
                bin = bin * (k - (seq_t)(j - 1)) / (seq_t)j;
                sum += bin * B[i - j];
            }
            B[i] = (seq_t)(-sum);
        }

        /* Compute A[i] for i > 0. */
        if (i > 0) {
            seq_t sum = 0;
            for (idx_t j = 1; j <= i; ++j) {
                sum += A[i - j] * B[j];
            }
            A[i] = (seq_t)(-sum);
        }
    }

    sz = n + 1;
    return A[n];
}
