/*
 * A355409  Expansion of e.g.f. 1/(1 + exp(2*x) - exp(3*x)).
 * https://oeis.org/A355409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A355409
   Formula: a(0) = 1;
            a(n) = Sum_{k=1..n} (3^k - 2^k) * binomial(n,k) * a(n-k)   (n ≥ 1)
*/

#include <stdlib.h>

static seq_t *A = NULL;
static seq_t *p2 = NULL;
static seq_t *p3 = NULL;
static size_t CAP = 0;          /* allocated size of the arrays          */
static idx_t    MAXN = 0;        /* highest n for which a[n] is computed */
static idx_t    PWR_MAX = 0;     /* highest k for which powers are cached */

/* enlarge the arrays if needed */
static void ensure_capacity(idx_t n) {
    if (n < CAP) return;
    size_t new_cap = (n + 1) * 2;
    if (new_cap < 16) new_cap = 16;
    A  = (seq_t *)realloc(A,  new_cap * sizeof(seq_t));
    p2 = (seq_t *)realloc(p2, new_cap * sizeof(seq_t));
    p3 = (seq_t *)realloc(p3, new_cap * sizeof(seq_t));
    CAP = new_cap;
}

/* cache powers of 2 and 3 up to n */
static void extend_powers(idx_t n) {
    if (n <= PWR_MAX) return;
    for (idx_t i = PWR_MAX + 1; i <= n; ++i) {
        p2[i] = p2[i-1] * 2;
        p3[i] = p3[i-1] * 3;
    }
    PWR_MAX = n;
}

/* main function to return a(n) */
seq_t a(idx_t n) {
    /* one‑time initialisation */
    if (CAP == 0) {
        CAP = 16;
        A  = (seq_t *)malloc(CAP * sizeof(seq_t));
        p2 = (seq_t *)malloc(CAP * sizeof(seq_t));
        p3 = (seq_t *)malloc(CAP * sizeof(seq_t));
        A[0]  = 1;
        p2[0] = 1;
        p3[0] = 1;
        PWR_MAX = 0;
        MAXN = 0;
    }

    /* make sure storage is big enough and powers are ready */
    ensure_capacity(n);
    extend_powers(n);

    /* compute missing terms up to n */
    for (idx_t i = MAXN + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;          /* C(i,0) */
        for (idx_t k = 1; k <= i; ++k) {
            /* update binomial coefficient C(i,k) incrementally */
            binom = (binom * (seq_t)(i - k + 1)) / (seq_t)k;
            seq_t coeff = p3[k] - p2[k];
            seq_t term = binom;
            term *= coeff;
            term *= A[i - k];
            sum += term;
        }
        A[i] = sum;
    }
    MAXN = n;
    return A[n];
}
