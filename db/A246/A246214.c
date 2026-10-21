/*
 * A246214  Number of endofunctions on [n] where the largest cycle length
 *          equals 4.
 * https://oeis.org/A246214
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of endofunctions on [n] where the largest cycle
 *            length equals 4. It is obtained as a(n) = f_4(n) – f_3(n), where
 *            f_m(n) counts endofunctions with all cycle lengths ≤ m. f_m(n)
 *            satisfies the recurrence f_m(0) = 1, f_m(n) = Σ_{k=1}^{n}
 *            C(n‑1,k‑1) · comp_m(k) · f_m(n‑k), with comp_m(k)...
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Make sure we can use memory and arbitrary‑precision operations */
#include <stddef.h>     /* NULL, size_t */
#include <stdlib.h>     /* malloc, realloc, free, memcpy */
#include <string.h>     /* memset, memcpy */

/* Helper function for huge exponentiation:  base^exp  (exp fits in idx_t).  */
#include "../utils/big_pow.h"

/* Utility: Create an array of seq_t, zero initialized */
static seq_t *seq_alloc(size_t sz)
{
    seq_t *p = (seq_t *)malloc(sz * sizeof(seq_t));
    if (p) memset(p, 0, sz * sizeof(seq_t));
    return p;
}

/* Utility: Reallocate an array of seq_t, extend to new size */
static seq_t *seq_realloc(seq_t *p, size_t new_sz)
{
    return (seq_t *)realloc(p, new_sz * sizeof(seq_t));
}

/* Global state for factorials */
static idx_t fact_cap = 0;
static seq_t *fact = NULL;
static void ensure_fact(idx_t N)
{
    if (N <= fact_cap) return;
    seq_t *new_fact = seq_alloc(N + 1);
    if (fact) {
        memcpy(new_fact, fact, (fact_cap + 1) * sizeof(seq_t));
        free(fact);
    }
    new_fact[0] = 1;
    for (idx_t i = 1; i <= N; ++i) {
        new_fact[i] = new_fact[i - 1] * (seq_t)i;
    }
    fact = new_fact;
    fact_cap = N;
}

/* Global state for component counts for a given m */
typedef struct {
    idx_t cap;      /* size of allocated array   */
    idx_t m;        /* cycle length limit used  */
    seq_t *arr;     /* comp_m(k) values             */
} comp_state;
static comp_state comp3 = {0, 0, NULL};
static comp_state comp4 = {0, 0, NULL};

/* Compute component table for a specific m */
static void ensure_comp(comp_state *st, idx_t m, idx_t N)
{
    if (st->cap >= N && st->m == m) return;
    /* free previous data if any */
    if (st->arr) free(st->arr);
    st->arr = seq_alloc(N + 1);
    st->cap = N;
    st->m = m;

    for (idx_t k = 1; k <= N; ++k) {
        seq_t sum = 0;
        idx_t lim = (k < m) ? k : m;
        for (idx_t j = 1; j <= lim; ++j) {
            if (j < k) {
                /* term = C(k,j) * j! * k^{k-j-1} */
                /* C(k,j) */
                seq_t comb = 1;
                for (idx_t ii = 1; ii <= j; ++ii) {
                    comb = comb * (k - ii + 1) / ii;
                }
                /* j! */
                seq_t term = comb * fact[j];
                /* power k^{k-j-1} */
                seq_t powv = big_pow((seq_t)k, (idx_t)(k - j - 1));
                term = term * powv;
                sum += term;
            } else { /* j == k */
                /* term = (k-1)! */
                sum += fact[k - 1];
            }
        }
        st->arr[k] = sum;
    }
}

/* Global state for f_m arrays */
typedef struct {
    idx_t cap;   /* current size allocated       */
    idx_t m;     /* m value for which we store (3 or 4) */
    seq_t *arr;  /* f_m(n) values                 */
} f_state;
static f_state f3 = {0, 0, NULL};
static f_state f4 = {0, 0, NULL};

/* Compute f_m(n) DP up to N for given m */
static void ensure_f(f_state *st, const comp_state *comp, idx_t N)
{
    if (st->cap >= N && st->m == comp->m) return;

    if (st->arr) free(st->arr);
    st->arr = seq_alloc(N + 1);
    st->cap = N;
    st->m = comp->m;

    /* DP: f(0)=1 */
    st->arr[0] = 1;

    for (idx_t n = 1; n <= N; ++n) {
        seq_t sum = 0;
        seq_t comb = 1; /* C(n-1,0) initially */
        for (idx_t k = 1; k <= n; ++k) {
            seq_t term = comp->arr[k];
            term = term * st->arr[n - k];
            term = term * comb;
            sum += term;
            if (k < n) {
                comb = comb * (n - k) / k;
            }
        }
        st->arr[n] = sum;
    }
}

/* Ensure that f_3 and f_4 tables are extended up to n */
static void extend_tables(idx_t n)
{
    if (n == 0) return;
    ensure_fact(n);
    ensure_comp(&comp3, 3, n);
    ensure_comp(&comp4, 4, n);
    ensure_f(&f3, &comp3, n);
    ensure_f(&f4, &comp4, n);
}

/* Public function required by the problem */
seq_t a(idx_t n)
{
    if (n < 4) return 0;            /* below offset, behaviour irrelevant */
    extend_tables(n);
    return f4.arr[n] - f3.arr[n];
}
