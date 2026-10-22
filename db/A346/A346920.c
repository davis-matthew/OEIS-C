/*
 * A346920  Expansion of e.g.f. 1 / (1 - (exp(x) - 1)^5 / 5!).
 * https://oeis.org/A346920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = Sum_{k=5..n} binomial(n,k) * Stirling2(k,5) *
 *            a(n-k) (n >= 1)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Stirling2(k,5) can be computed with the recurrence
    S(n,k) = k * S(n-1,k) + S(n-1,k-1).                          */

#include <stddef.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Dynamic array helpers                                               */
/* --------------------------------------------------------------------- */
static void expand_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = *cap ? *cap * 2 : 1;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    *arr = tmp;
    *cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Sequence storage                                                     */
/* --------------------------------------------------------------------- */
static seq_t *a_cache   = NULL;   /* a[0..max_a]            */
static size_t  a_cap    = 0;
static idx_t   max_a    = 0;

static seq_t *S5_cache  = NULL;   /* Stirling2(k,5) 0..max_a */
static size_t  S5_cap   = 0;

/* --------------------------------------------------------------------- */
/*  Current Stirling row (S(n,0..5))                                     */
/* --------------------------------------------------------------------- */
static seq_t stirling[6] = { 0 };

/* --------------------------------------------------------------------- */
/*  Build tables up to n                                                 */
/* --------------------------------------------------------------------- */
static void compute_to_n(idx_t n)
{
    if (a_cache == NULL) {                 /* first call */
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        a_cap   = 1;
        a_cache[0] = (seq_t)1;             /* a(0)=1 */

        S5_cache = (seq_t *)malloc(sizeof(seq_t));
        S5_cap   = 1;
        S5_cache[0] = (seq_t)0;             /* S(0,5)=0 */

        stirling[0] = (seq_t)1;             /* S(0,0)=1, others 0 */
        for (int j = 1; j <= 5; ++j) stirling[j] = (seq_t)0;
        max_a = 0;
    }

    if (n <= max_a) return;                /* nothing to do */

    expand_array(&a_cache,  &a_cap,   n + 1);
    expand_array(&S5_cache, &S5_cap, n + 1);

    for (idx_t i = max_a + 1; i <= n; ++i) {
        /* ---------------------------------------------------------- */
        /*  Build Stirling2(i,0..5) from previous row                 */
        /* ---------------------------------------------------------- */
        seq_t new_st[6];
        new_st[0] = (seq_t)0;
        for (int j = 1; j <= 5; ++j) {
            /* S(i,j) = j * S(i-1,j) + S(i-1,j-1) */
            new_st[j] = stirling[j] * (seq_t)j + stirling[j-1];
        }
        S5_cache[i] = new_st[5];          /* store S(i,5) */
        for (int j = 0; j <= 5; ++j) stirling[j] = new_st[j];

        /* ---------------------------------------------------------- */
        /*  Compute a(i) by the recurrence                           */
        /* ---------------------------------------------------------- */
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;          /* binomial(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* update binom: binom = C(i,k) */
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k;
            if (k >= 5) {
                seq_t prod = binom * S5_cache[k];
                prod = prod * a_cache[i - k];
                sum = sum + prod;
            }
        }
        a_cache[i] = sum;
    }

    max_a = n;
}

/* --------------------------------------------------------------------- */
/*  Public interface                                                    */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    compute_to_n(n);
    return a_cache[n];
}
