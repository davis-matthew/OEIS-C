/*
 * A281779  Number of distinct topologies on an n-set that have exactly 11
 *          open sets.
 * https://oeis.org/A281779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25/6*5! · S2(n,5) + 79/6*6! · S2(n,6) + 29/2*7! · S2(n,7)
 *            + 39/4*8! · S2(n,8) + 4*9! · S2(n,9) + 10! · S2(n,10)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  S2 is the Stirling number of the second kind.  */
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Number of terms that are needed in the Stirling table.  */
#define KMAX 10

/* Linear combination coefficients for k = 5 … 10.  */
static const seq_t coeff[KMAX + 1] = {
    0, 0, 0, 0, 0,               /* k = 0 … 4  */
    (seq_t)500,                  /* 25/6 * 5!  */
    (seq_t)9480,                 /* 79/6 * 6!  */
    (seq_t)73080,                /* 29/2 * 7!  */
    (seq_t)393120,               /* 39/4 * 8!  */
    (seq_t)1451520,              /* 4  * 9!   */
    (seq_t)3628800               /* 1  *10!   */
};

/* Pointer to a 1‑dimensional array that holds S[n][k] for 0 ≤ k ≤ KMAX
   for all computed n.  Each element is of type seq_t.  */
static seq_t *S = NULL;
/* Largest n for which the table has been filled. */
static idx_t max_n = 0;

/* Ensure that Stirling numbers S[n][k] are available for all  
   n ≤ n_target.  The array is expanded as necessary.  */
static void compute_stirling_up_to(idx_t n_target)
{
    if (S != NULL && max_n >= n_target)
        return;                     /* already computed */

    /* New size for the table: (n_target+1) rows, each with KMAX+1 columns. */
    size_t new_rows = (size_t)(n_target + 1);
    size_t new_size = new_rows * (KMAX + 1);
    seq_t *tmp = NULL;

    if (S == NULL) {                 /* first allocation */
        tmp = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        /* Construct each element (necessary when seq_t is a class). */
#ifdef __cplusplus
        for (size_t i = 0; i < new_size; ++i)
            new (&tmp[i]) seq_t();
#endif
        /* Zero the whole array – calloc already did it, but we re‑assign
           to guarantee the value for class types. */
        for (size_t i = 0; i < new_size; ++i)
            tmp[i] = (seq_t)0;
        tmp[0] = (seq_t)1;           /* S[0][0] = 1 */
        S = tmp;
        max_n = n_target;
    } else if (max_n + 1 < n_target + 1) {   /* enlarge the table */
        size_t old_size = (max_n + 1) * (KMAX + 1);
        tmp = (seq_t *)realloc(S, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        /* Construct the newly allocated part. */
#ifdef __cplusplus
        for (size_t i = old_size; i < new_size; ++i)
            new (&tmp[i]) seq_t();
#endif
        /* Zero the new entries. */
        for (size_t i = old_size; i < new_size; ++i)
            tmp[i] = (seq_t)0;
        S = tmp;
    }

    /* Compute rows that were not yet filled.  */
    for (idx_t n = max_n + 1; n <= n_target; ++n) {
        size_t base_cur  = (size_t)n * (KMAX + 1);
        size_t base_prev = (size_t)(n - 1) * (KMAX + 1);
        /* S[n][0] stays 0. */
        for (int k = 1; k <= KMAX; ++k) {
            seq_t term1 = S[base_prev + k] * (seq_t)k;
            seq_t term2 = S[base_prev + (k - 1)];
            S[base_cur + k] = term1 + term2;
        }
    }
    max_n = n_target;
}

/* Compute a(n) for the OEIS sequence A281779.  */
seq_t a(idx_t n)
{
    compute_stirling_up_to(n);
    size_t base = (size_t)n * (KMAX + 1);
    seq_t result = (seq_t)0;
    for (int k = 5; k <= KMAX; ++k) {
        result += coeff[k] * S[base + k];
    }
    return result;
}
