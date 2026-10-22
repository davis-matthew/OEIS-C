/*
 * A381107  Expansion of e.g.f. -log(1-x) * (exp(x) - 1) / (1-x).
 * https://oeis.org/A381107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n,k) * |Stirling1(k+1,2)| Using
 *            the identity |Stirling1(m,2)| = (m-1)! * H_{m-1} and the derived
 *            recurrence for N_m = m*N_{m-1} + 1 with N_1 = 1, we compute a(n)
 *            = Sum_{m=1..n-1} N_m * binomial(n,m) * factorial(n-m-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Automatic caching of factorials and N values */
static idx_t  max_n_computed = 0;
static seq_t *fact_arr       = NULL;
static seq_t *N_arr          = NULL;
static size_t  arr_capacity   = 0;

/* initialise or extend the cached arrays up to index n */
static void ensure_cache(idx_t n)
{
    if (n <= max_n_computed) return;           /* already have enough */

    /* enlarge the storage */
    size_t new_cap = n + 1;                     /* minimal capacity */
    if (new_cap < 2) new_cap = 2;
    if (arr_capacity < new_cap) {
        seq_t *new_fact = (seq_t*)realloc(fact_arr, new_cap * sizeof(seq_t));
        seq_t *new_N    = (seq_t*)realloc(N_arr,    new_cap * sizeof(seq_t));
        if (!new_fact || !new_N) {
            /* allocation failed – abort */
            exit(EXIT_FAILURE);
        }
        fact_arr = new_fact;
        N_arr    = new_N;
        arr_capacity = new_cap;
    }

    /* fill data from old bound+1 up to n */
    if (max_n_computed == 0) {
        fact_arr[0] = (seq_t)1;   /* 0! = 1 */
        N_arr[0]    = (seq_t)0;   /* unused but defined */
    }
    for (idx_t i = max_n_computed + 1; i <= n; ++i) {
        fact_arr[i] = fact_arr[i-1] * (seq_t)i;   /* factorial recurrence          */
        if (i == 1) {
            N_arr[i] = (seq_t)1;                 /* N_1 = 1                        */
        } else {
            N_arr[i] = (seq_t)i * N_arr[i-1] + (seq_t)1; /* N_m = m*N_{m-1} + 1   */
        }
    }
    max_n_computed = n;   /* all values up to n are now available */
}

/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* the requested n is up to 10000 – precompute once up to that */
    if (n > (idx_t)10000) ensure_cache((idx_t)10000);
    else                  ensure_cache(n);

    seq_t sum = (seq_t)0;
    if (n <= 1) return sum;     /* a(0)=a(1)=0 */

    seq_t bin = (seq_t)1;        /* C(n,0) = 1 */
    for (idx_t m = 1; m <= n - 1; ++m) {
        /* update binomial coefficient */
        bin = bin * (seq_t)(n - m + 1) / (seq_t)m;   /* C(n,m) from C(n,m-1) */

        /* term = N_m * C(n,m) * (n-m-1)! */
        seq_t term = N_arr[m] * bin;
        if (n - m - 1 > 0) {
            term = term * fact_arr[n - m - 1];
        }
        sum += term;
    }
    return sum;
}
