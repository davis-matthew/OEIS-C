/*
 * A363817  G.f. satisfies A(x) = (1 + x/A(x))/(1 - x)^3.
 * https://oeis.org/A363817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; For n ≥ 1 a(n) = (n==1?1:0) + 3*C(n‑1) – 3*C(n‑2) +
 *            C(n‑3) – Σ_{i=1}^{n‑1} a(i)*a(n‑i) where C(k) = Σ_{i=0}^k
 *            a(i)*a(k‑i), and C(‑1)=C(‑2)=0
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 15, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External integer types supplied by the build */
extern seq_t a(idx_t n);

static seq_t *a_arr = NULL;   /* sequence values */
static seq_t *c_arr = NULL;   /* convolution values C(k) */
static size_t cap = 0;        /* current allocated capacity */
static idx_t len = 0;         /* number of computed terms */

/* Allocate / enlarge the static arrays to hold indices up to n. */
static void ensure_capacity(idx_t n)
{
    if (cap > (size_t)n) return;

    size_t new_cap = cap ? (cap << 1) : 4;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *new_a = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    seq_t *new_c = (seq_t *)realloc(c_arr, new_cap * sizeof(seq_t));
    if (!new_a || !new_c) abort();

    /* Initialize newly allocated elements to zero. */
    for (size_t i = cap; i < new_cap; ++i) {
        new_a[i] = 0;
        new_c[i] = 0;
    }

    a_arr = new_a;
    c_arr = new_c;
    cap = new_cap;
}

/* Sequence generator function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* First initialization of a(0) and C(0). */
    if (len == 0) {
        ensure_capacity(0);
        a_arr[0] = (seq_t)1;
        c_arr[0] = (seq_t)1; /* C(0) = a0*a0 = 1 */
        len = 1;
    }

    if (n < len) return a_arr[n];

    ensure_capacity(n);

    /* Compute terms from current length up to n. */
    for (idx_t k = len; k <= n; ++k) {
        /* Compute the sum S = Σ_{i=1}^{k‑1} a(i)*a(k‑i). */
        seq_t S = 0;
        for (idx_t i = 1; i < k; ++i) {
            S += a_arr[i] * a_arr[k - i];
        }

        /* rhs = 1 if k == 1 else 0. */
        seq_t rhs = (k == 1) ? (seq_t)1 : (seq_t)0;

        /* a(k) = rhs + 3*C(k‑1) – 3*C(k‑2) + C(k‑3) – S. */
        seq_t an = rhs;
        an += (seq_t)3 * c_arr[k - 1];
        if (k >= 2) an -= (seq_t)3 * c_arr[k - 2];
        if (k >= 3) an += c_arr[k - 3];
        an -= S;

        a_arr[k] = an;

        /* C(k) = Σ_{i=0}^k a(i)*a(k‑i) = 2*a(k) + S. */
        c_arr[k] = (seq_t)2 * an + S;
    }

    len = n + 1;
    return a_arr[n];
}
