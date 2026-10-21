/*
 * A245858  Number of preferential arrangements of n labeled elements such
 *          that the minimal number of elements per rank equals 5.
 * https://oeis.org/A245858
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A245790(n) - A245791(n)
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 19, u64 n <= 29, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for a set size restriction m: */
/*   f_m(0) = 1,                                   */
/*   f_m(n) = Σ_{k=m}^{n} C(n, k) * f_m(n‑k)      */
/*   a(n)   = f_5(n) – f_6(n) (for n ≥ 5)          */

#include <stddef.h>
#include <stdint.h>

/* External types supplied by the build */
typedef uint64_t idx_t;

/* Internal storage for the two recurrences */
static seq_t *F5 = NULL, *F6 = NULL;
static size_t capF5 = 0, capF6 = 0;
static idx_t  maxF5 = 0, maxF6 = 0;

/* Ensure the array can hold indices up to 'need' */
static void ensure_array(idx_t need, seq_t **arr, size_t *cap, idx_t *max)
{
    if (*arr == NULL) {                     /* first allocation */
        size_t new_cap = need + 1;
        if (new_cap < 1) new_cap = 1;
        *arr  = (seq_t *)calloc(new_cap, sizeof(seq_t));
        *cap  = new_cap;
        *max  = 0;
        (*arr)[0] = (seq_t)1;               /* base value a(0) = 1 */
        return;
    }
    if (need >= (idx_t)(*cap)) {           /* grow the array */
        size_t new_cap = *cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)need) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        for (idx_t i = *max + 1; i <= need; ++i) (*arr)[i] = 0;
        *cap = new_cap;
    }
}

/* Compute f_m for indices up to N using the recurrence */
static void compute_f(idx_t N, int m, seq_t *F, idx_t *max)
{
    if (N <= *max) return;                /* already computed */

    for (idx_t n = *max + 1; n <= N; ++n) {
        if (n < (idx_t)m) {
            F[n] = (seq_t)0;                 /* no block can meet the size */
            continue;
        }

        /* initialise binomial coeff C(n, m) */
        seq_t c = 1;
        for (idx_t i = 0; i < (idx_t)m; ++i) {
            c = c * (seq_t)(n - i) / (seq_t)(i + 1);
        }

        seq_t sum = 0;
        for (idx_t k = m; k <= n; ++k) {
            sum += c * F[n - k];

            if (k < n) {                  /* move to C(n, k+1) */
                seq_t num = n - k;
                seq_t den = k + 1;
                c = (c * num) / den;
            }
        }
        F[n] = sum;
    }
    *max = N;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;                 /* offset behaviour */

    /* Grow arrays and compute up to n */
    ensure_array(n, &F5, &capF5, &maxF5);
    compute_f(n, 5, F5, &maxF5);

    ensure_array(n, &F6, &capF6, &maxF6);
    compute_f(n, 6, F6, &maxF6);

    return F5[n] - F6[n];
}
