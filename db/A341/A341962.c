/*
 * A341962  G.f. B(x) satisfies: B(x) = (1 - x^2*B(x)^2) / (1 - 2*x*B(x))^2.
 * https://oeis.org/A341962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence derived from the algebraic equation for B(x):
   let a[n] (n≥0) be the coefficients of B(x)  = Σ a[n] x^n.
   Define auxiliary sequences
       c[n] = Σ_{k=0..n} a[k] * a[n-k]          (convolution of a with itself)
       d[n] = Σ_{k=0..n} a[k] * c[n-k]          (convolution of a with c)

   Then for n ≥ 1
       a[n] = 4*c[n-1] - c[n-2] - 4*d[n-2]
   with initial values
       a[0] = 1
       c[0] = a[0]*a[0] = 1
       d[0] = a[0]*c[0] = 1
   (For indices <0, the corresponding c or d values are taken as 0.)

   This recurrence yields the OEIS sequence A341962.
*/
#include <stdlib.h>
#include <string.h>

/* static caches for the sequence and auxiliary arrays */
static seq_t *a_seq = NULL;   /* a[n] */
static seq_t *c_seq = NULL;   /* c[n] */
static seq_t *d_seq = NULL;   /* d[n] */
static size_t  cap  = 0;      /* current capacity of the arrays */
static idx_t   max_n = (idx_t)-1;   /* highest index that has been computed */

/* enlarge the cached arrays to hold indices up to n */
static void ensure_cap(idx_t n) {
    size_t need = (size_t)n + 1;
    if (need <= cap) return;                /* already large enough */

    size_t new_cap = need * 2;               /* double to amortize reallocs */
    if (new_cap < need) new_cap = need;     /* avoid overflow */

    a_seq = (seq_t *)realloc(a_seq, new_cap * sizeof(*a_seq));
    c_seq = (seq_t *)realloc(c_seq, new_cap * sizeof(*c_seq));
    d_seq = (seq_t *)realloc(d_seq, new_cap * sizeof(*d_seq));

    /* initialise the newly allocated part to zero (not strictly required) */
    memset(a_seq + cap, 0, (new_cap - cap) * sizeof(*a_seq));
    memset(c_seq + cap, 0, (new_cap - cap) * sizeof(*c_seq));
    memset(d_seq + cap, 0, (new_cap - cap) * sizeof(*d_seq));

    cap = new_cap;
}

/* compute a(n), memoising results for any order of calls */
seq_t a(idx_t n)
{
    /* initialise caches on first call */
    if (max_n == (idx_t)-1) {
        ensure_cap(0);
        a_seq[0] = 1;
        c_seq[0] = 1;
        d_seq[0] = 1;
        max_n   = 0;
    }

    /* return already‑computed value */
    if (n <= max_n) return a_seq[n];

    /* extend the computation to the requested n */
    ensure_cap(n);
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* auxiliary values needed for the recurrence */
        seq_t c_n1 = (i >= 1) ? c_seq[i-1] : 0;
        seq_t c_n2 = (i >= 2) ? c_seq[i-2] : 0;
        seq_t d_n2 = (i >= 2) ? d_seq[i-2] : 0;

        /* a[i] = 4*c[i-1] - c[i-2] - 4*d[i-2]   (i≥1)  */
        seq_t val = 4 * c_n1;
        val -= c_n2;
        val -= 4 * d_n2;
        a_seq[i] = val;

        /* compute c[i] = Σ a[k]*a[i-k] */
        seq_t sum_c = 0;
        for (idx_t k = 0; k <= i; ++k)
            sum_c += a_seq[k] * a_seq[i-k];
        c_seq[i] = sum_c;

        /* compute d[i] = Σ a[k]*c[i-k] */
        seq_t sum_d = 0;
        for (idx_t k = 0; k <= i; ++k)
            sum_d += a_seq[k] * c_seq[i-k];
        d_seq[i] = sum_d;
    }

    max_n = n;
    return a_seq[n];
}
