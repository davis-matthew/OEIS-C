/*
 * A307595  Expansion of e.g.f. (sec(x) + tan(x))*exp(x/(1 - x)).
 * https://oeis.org/A307595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n/n! in
 *            (sec(x)+tan(x))*exp(x/(1-x)). It equals the Boustrophedon
 *            transform of A000262. The sequence is produced via the three
 *            auxiliary sequences: E(n) – coefficients of sec(x)+tan(x) U(n) –
 *            coefficients of exp(x/(1-x)) a(n) – convolution of E and U. The
 *            following im...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, realloc, free */
#include <stdbool.h>         /* bool type */

static seq_t *E = NULL, *U = NULL, *A = NULL;   /* caches */
static size_t cap = 0;                          /* current capacity */

/* Expand the caches up to index n (inclusive).  */
static void ensure_size(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cap) return;                    /* nothing to do */

    /* grow capacity (at least double for amortized O(n)) */
    size_t new_cap = cap > 0 ? cap : 1;
    while (new_cap < need) new_cap <<= 1;

    E = (seq_t *)realloc(E, new_cap * sizeof(seq_t));
    U = (seq_t *)realloc(U, new_cap * sizeof(seq_t));
    A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    if (!E || !U || !A) {
        /* allocation failed – abort */
        exit(1);
    }

    /* initialise the base values */
    if (cap == 0) {
        E[0] = (seq_t)1;
        U[0] = (seq_t)1;
        A[0] = (seq_t)1;
    }

    /* compute values for indices >= cap */
    for (size_t i = cap; i < need; ++i) {
        /* ---- compute E[i] (sec+tan coefficients) ---- */
        if (i == 0) {
            /* already set */
        } else if (i == 1) {
            E[i] = (seq_t)1;
        } else {
            seq_t sum = 0;
            seq_t binom = 1;                     /* binom(i-1,0) */
            for (size_t k = 0; k <= i-1; ++k) {
                seq_t term = binom * E[k] * E[i-1-k];
                sum += term;
                if (k < i-1) {
                    binom = binom * (seq_t)(i-1-k) / (seq_t)(k+1);
                }
            }
            E[i] = sum / 2;                      /* sum is even */
        }

        /* ---- compute U[i] (exp(x/(1-x)) coefficients) ---- */
        if (i == 0) {
            /* already set */
        } else {
            seq_t sum = 0;
            seq_t prod = 1;                     /* product for k = i-1 is 1 */
            for (size_t k = i-1; ; --k) {
                seq_t temp = U[k];
                temp *= (seq_t)(i - k);
                temp *= prod;
                sum += temp;
                if (k == 0) break;
                prod *= (seq_t)k;
            }
            U[i] = sum;
        }

        /* ---- compute a[i] via convolution ---- */
        if (i == 0) {
            /* already set */
        } else {
            seq_t sum = 0;
            seq_t binom = 1;                     /* binom(i,0) */
            for (size_t k = 0; k <= i; ++k) {
                seq_t term = binom * E[k] * U[i-k];
                sum += term;
                if (k < i) {
                    binom = binom * (seq_t)(i-k) / (seq_t)(k+1);
                }
            }
            A[i] = sum;
        }
    }
    cap = need;
}

/* public entry point – returns a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    ensure_size(n);
    return A[n];
}
