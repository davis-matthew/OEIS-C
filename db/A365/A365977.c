/*
 * A365977  Expansion of e.g.f. 1 / ( 1 - Sum_{k>=0} x^(5*k+1) / (5*k+1) ).
 * https://oeis.org/A365977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=0..⌊(n-1)/5⌋} (5k)! · C(n,5k+1) ·
 *            a(n-5k-1) (OEIS recurrence for A365977)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---------- local helper: multiplicative binomial ---------- */
static seq_t binomial_calc(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* ---------- factorial cache ---------- */
static seq_t *fact = NULL;        /* fact[i] = i! */
static size_t fact_cap = 0;       /* allocated size of fact */

static void ensure_fact_capacity(idx_t n)
{
    if (n == (idx_t)0 && fact_cap == 0) return;
    if (fact_cap > n) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    fact = tmp;
    if (fact_cap == 0) {
        fact[0] = 1;
        fact_cap = 1;
    }
    for (idx_t i = fact_cap; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_cap = new_cap;
}

/* ---------- sequence cache ---------- */
static seq_t *A = NULL;           /* A[i] = a(i) */
static size_t a_cap = 0;          /* allocated size of A */
static idx_t computed = 0;        /* highest index computed so far */

static void ensure_a_capacity(idx_t n)
{
    if (a_cap > (size_t)n) return;
    size_t new_cap = (size_t)(n + 1) * 2;
    if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    A = tmp;
    a_cap = new_cap;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* initialise once */
    if (A == NULL) {
        /* allocate minimal space for index 0 */
        a_cap = 1;
        A = (seq_t *)malloc(sizeof(seq_t));
        if (!A) abort();
        A[0] = 1;
        computed = 0;

        fact_cap = 1;
        fact = (seq_t *)malloc(sizeof(seq_t));
        if (!fact) abort();
        fact[0] = 1;
    }

    if (n == 0) return A[0];

    /* ensure factorials up to n-1 are prepared */
    ensure_fact_capacity(n - 1);

    /* make sure DP array can hold index n */
    ensure_a_capacity(n);

    /* compute new entries up to n */
    for (idx_t m = computed + 1; m <= n; ++m) {
        seq_t sum = 0;
        idx_t maxk = (m - 1) / 5;
        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t idx5 = 5 * k;          /* 5*k */
            seq_t r = fact[idx5];        /* (5*k)! */
            r = r * binomial_calc(m, idx5 + 1); /* * C(m,5k+1) */
            r = r * A[m - idx5 - 1];     /* * a(m-5k-1) */
            sum += r;
        }
        A[m] = sum;
    }

    computed = n;
    return A[n];
}
