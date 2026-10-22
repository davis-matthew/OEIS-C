/*
 * A356576  Expansion of e.g.f. ( Product_{k>0} (1+x^k)^(1/k!) )^x.
 * https://oeis.org/A356576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=0; a(n)=Sum_{k=2..n}
 *            k*A352013(k-1)*binomial(n-1,k-1)*a(n-k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static state – memoised values */
static seq_t *fact_arr = NULL;   /* factorials: fact_arr[i] = i!          */
static seq_t *A352_arr = NULL;   /* A352013 values: A352_arr[m]            */
static seq_t *B_arr    = NULL;   /* B[k] = k * A352013(k-1)               */
static seq_t *a_arr    = NULL;   /* sequence values: a_arr[n] = a(n)      */
static idx_t max_n     = 0;      /* highest n whose a[n] is known        */

/* Helper: compute A352013(m) using pre‑computed factorials */
static seq_t compute_A352(idx_t m)
{
    if (m == 0) return 0;
    seq_t sum = 0;
    seq_t fact_m1 = fact_arr[m-1];
    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            /* divisor d */
            idx_t t = d;
            idx_t q = m / t;        /* m/t */
            int sign = (q & 1U) ? 1 : -1;   /* (-1)^{q+1} */
            seq_t term = fact_m1 / fact_arr[t-1];
            term = term * sign;
            sum = sum + term;

            /* complementary divisor */
            idx_t e = m / d;
            if (e != d) {
                t = e;
                q = m / t;
                sign = (q & 1U) ? 1 : -1;
                term = fact_m1 / fact_arr[t-1];
                term = term * sign;
                sum = sum + term;
            }
        }
    }
    return sum;
}

/* Extend all tables so that a[n] is known for 0 <= n <= need */
static void ensure_capacity(idx_t need)
{
    if (need <= max_n) return;          /* already have enough */

    /* new size needed (indices 0..need) */
    idx_t new_cap = need + 1;

    /* ---- factorials ---- */
    if (!fact_arr) {
        fact_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
        fact_arr[0] = 1;
    } else {
        fact_arr = (seq_t*)realloc(fact_arr, new_cap * sizeof(seq_t));
    }
    for (idx_t i = max_n + 1; i < new_cap; ++i)
        fact_arr[i] = fact_arr[i-1] * (seq_t)i;

    /* ---- allocate A352 and B arrays ---- */
    if (!A352_arr) {
        A352_arr = (seq_t*)calloc(new_cap, sizeof(seq_t));
    } else {
        A352_arr = (seq_t*)realloc(A352_arr, new_cap * sizeof(seq_t));
    }
    if (!B_arr) {
        B_arr = (seq_t*)calloc(new_cap, sizeof(seq_t));
    } else {
        B_arr = (seq_t*)realloc(B_arr, new_cap * sizeof(seq_t));
    }

    /* ---- allocate a array ---- */
    if (!a_arr) {
        a_arr = (seq_t*)calloc(new_cap, sizeof(seq_t));
        a_arr[0] = 1;          /* a(0) */
        if (new_cap > 1) a_arr[1] = 0; /* a(1) */
    } else {
        a_arr = (seq_t*)realloc(a_arr, new_cap * sizeof(seq_t));
    }

    /* compute new A352 values and corresponding B values */
    for (idx_t m = max_n + 1; m < new_cap; ++m)
        A352_arr[m] = compute_A352(m);
    for (idx_t k = 2; k < new_cap; ++k)
        B_arr[k] = (seq_t)k * A352_arr[k-1];

    /* compute a[n] for new indices (starting from at least 2) */
    idx_t start = (max_n + 1 > 1 ? max_n + 1 : 2);
    for (idx_t n = start; n < new_cap; ++n) {
        seq_t sum = 0;
        /* binomial coefficient C(n-1,1) = n-1  */
        seq_t binom = (seq_t)(n - 1);
        for (idx_t k = 2; k <= n; ++k) {
            seq_t term = B_arr[k];
            term = term * binom;
            term = term * a_arr[n - k];
            sum = sum + term;
            if (k < n) {
                /* update to C(n-1, k) from C(n-1, k-1) */
                binom = (binom * (seq_t)(n - k)) / (seq_t)k;
            }
        }
        a_arr[n] = sum;
    }

    max_n = need;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;

    if (n > max_n)
        ensure_capacity(n);
    return a_arr[n];
}
