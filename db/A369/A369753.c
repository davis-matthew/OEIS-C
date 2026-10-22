/*
 * A369753  Expansion of e.g.f. exp(1 - (1+x)^5).
 * https://oeis.org/A369753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = -5*(n-1)! * Sum_{k=1..min(5,n)} C(4,k-1) *
 *            a(n-k) / (n-k)! (n≥1)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 17, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *A = NULL;      /* cache of a(n) */
static size_t Acap = 0;
static idx_t Amax = 0;

static seq_t *F = NULL;      /* cache of factorials */
static size_t Fcap = 0;
static idx_t Fmax = 0;

static const seq_t bin[5] = { (seq_t)1, (seq_t)4, (seq_t)6, (seq_t)4, (seq_t)1 };

seq_t a(idx_t n)
{
    /* allocate / grow factorial cache */
    if (F == NULL) {
        Fcap = (size_t)(n + 1);
        F = (seq_t *)realloc(NULL, Fcap * sizeof(seq_t));
        if (F == NULL) return 0;
        F[0] = (seq_t)1;
        Fmax = 0;
    } else if (n + 1 > Fcap) {
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(F, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0;
        F = tmp;
        Fcap = newcap;
    }

    if (Fmax < n) {
        for (idx_t i = Fmax + 1; i <= n; ++i)
            F[i] = F[i - 1] * (seq_t)i;
        Fmax = n;
    }

    /* allocate / grow a(n) cache */
    if (A == NULL) {
        Acap = (size_t)(n + 1);
        A = (seq_t *)realloc(NULL, Acap * sizeof(seq_t));
        if (A == NULL) return 0;
        A[0] = (seq_t)1;
        Amax = 0;
    } else if (n + 1 > Acap) {
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0;
        A = tmp;
        Acap = newcap;
    }

    if (n == 0) return A[0];

    /* compute missing values up to n */
    for (idx_t i = Amax + 1; i <= n; ++i) {
        size_t kmax = (i < 5 ? i : 5);
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= kmax; ++k) {
            idx_t sub = i - k;
            seq_t m = (seq_t)1;
            for (idx_t t = sub + 1; t < i; ++t)
                m *= (seq_t)t;          /* m = (i-1)! / (sub)!  */
            seq_t term = bin[k - 1] * A[sub] * m;
            sum += term;
        }
        A[i] = -5 * sum;
    }
    Amax = n;

    return A[n];
}
