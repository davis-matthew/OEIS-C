/*
 * A385806  G.f. A(x) satisfies A(x) = 1/(1 - x*A(x)^4 - x^2*A(x)^3*A'(x)).
 * https://oeis.org/A385806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i,j,k,l,m>=0, i+j+k+l+m=n-1}
 *            (i+1)a(i)a(j)a(k)a(l)a(m) with a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Forward declarations (seq_t and idx_t are provided by the build). */
static seq_t *a_seq = NULL;    /* a(n) values */
static seq_t *t_conv = NULL;   /* coefficients of a * a   */
static seq_t *s_conv = NULL;   /* coefficients of (a * a) * (a * a) = A^4 */

/* Amount of memory allocated for the arrays and the highest computed index. */
static size_t a_cap = 0;
static size_t t_cap = 0;
static size_t s_cap = 0;
static idx_t a_last = 0;

/* Ensure that all three arrays have room for at least (need+1) entries. */
static void ensure_capacity(idx_t need)
{
    if (a_cap >= (size_t)need + 1) return;

    size_t new_cap = a_cap ? a_cap << 1 : 16;
    if (new_cap <= (size_t)need) new_cap = (size_t)need + 1;
    /* Grow a_seq */
    a_seq = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
    /* Grow t_conv */
    t_conv = (seq_t *)realloc(t_conv, new_cap * sizeof(seq_t));
    /* Grow s_conv */
    s_conv = (seq_t *)realloc(s_conv, new_cap * sizeof(seq_t));
    /* Initialize new cells to zero */
    for (size_t i = a_cap; i < new_cap; ++i) {
        a_seq[i] = 0;
        t_conv[i] = 0;
        s_conv[i] = 0;
    }
    a_cap = t_cap = s_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initialise arrays on first call. */
    if (a_cap == 0) {
        ensure_capacity(0); /* allocate capacity 16 */
        a_seq[0]  = (seq_t)1;   /* a(0) = 1 */
        t_conv[0] = (seq_t)1;   /* a(0)^2 = 1 */
        s_conv[0] = (seq_t)1;   /* (a(0)^2)^2 = 1 */
        a_last   = 0;
    }

    /* Compute all missing terms up to n. */
    for (idx_t cur = a_last + 1; cur <= n; ++cur) {
        /* Ensure space for the new index. */
        ensure_capacity(cur);

        /* a(cur) = sum_{i=0}^{cur-1} (i+1) * a[i] * S[cur-1-i] */
        seq_t sum_a = (seq_t)0;
        idx_t target = cur - 1;
        for (idx_t i = 0; i < cur; ++i) {
            seq_t term = a_seq[i];
            term *= (seq_t)(i + 1);          /* (i+1) */
            term *= s_conv[target - i];
            sum_a += term;
        }
        a_seq[cur] = sum_a;

        /* T[cur] = sum_{i=0}^{cur} a[i] * a[cur-i]  (coefficients of A^2) */
        seq_t sum_t = (seq_t)0;
        for (idx_t i = 0; i <= cur; ++i) {
            sum_t += a_seq[i] * a_seq[cur - i];
        }
        t_conv[cur] = sum_t;

        /* S[cur] = sum_{p=0}^{cur} T[p] * T[cur-p]  (coefficients of A^4) */
        seq_t sum_s = (seq_t)0;
        for (idx_t p = 0; p <= cur; ++p) {
            sum_s += t_conv[p] * t_conv[cur - p];
        }
        s_conv[cur] = sum_s;
    }

    a_last = n;
    return a_seq[n];
}
