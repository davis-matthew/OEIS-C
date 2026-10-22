/*
 * A375684  Expansion of e.g.f. 1 / (1 - x * log(1 - x)).
 * https://oeis.org/A375684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; a(1)=0; a(n) = -n! * Σ_{k=2..n} a(n-k)/(n-k)!/(k-1)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_values      = NULL;  /* stores a(n) */
static seq_t *term_values     = NULL;  /* stores temporary terms for the recurrence */
static idx_t  cache_size      = 0;     /* highest index for which seq_values[0..cache_size-1] are valid */

/* Ensure internal buffers can hold indices up to n */
static void ensure_capacity(idx_t n)
{
    idx_t required = n + 1;
    if (required <= cache_size)
        return;

    /* Allocate new buffers */
    size_t newsize = (size_t)required;
    seq_t *new_seq  = (seq_t*)malloc(newsize * sizeof(seq_t));
    seq_t *new_term = (seq_t*)malloc(newsize * sizeof(seq_t));
    if (!new_seq || !new_term) {
        /* Allocation failed – terminate the program (not expected in the judge environment). */
        exit(1);
    }

    /* Copy old data */
    for (idx_t i = 0; i < cache_size; ++i) {
        new_seq[i]  = seq_values[i];
        new_term[i] = term_values[i];
    }

    /* Initialise new entries to zero */
    for (idx_t i = cache_size; i < newsize; ++i) {
        new_seq[i]  = (seq_t)0;
        new_term[i] = (seq_t)0;
    }

    /* First‑time initialisation */
    if (cache_size == 0) {
        new_seq[0] = (seq_t)1;          /* a(0) */
        if (newsize > 1) new_seq[1] = (seq_t)0;  /* a(1) */
        /* term_values[0] will be set during the first real recurrence step (m==2) */
    }

    /* Free old buffers and update globals */
    free(seq_values);
    free(term_values);
    seq_values  = new_seq;
    term_values = new_term;
    cache_size  = newsize;
}

seq_t a(idx_t n)
{
    /* Remember old cache size before reallocation */
    idx_t old_size = cache_size;

    ensure_capacity(n);

    /* Compute values that are not yet cached */
    for (idx_t m = old_size; m <= n; ++m) {
        if (m == 0 || m == 1)
            continue;                /* a(0) and a(1) are already known */

        /* For m==2 initialise term_values[0] */
        if (m == 2)
            term_values[0] = seq_values[0] * (seq_t)2;   /* term[0] = a(0) * 2 */

        /* Compute a(m) = - Σ_{j=0}^{m-2} term[j]  */
        seq_t sum = (seq_t)0;
        for (idx_t j = 0; j <= m - 2; ++j)
            sum += term_values[j];
        seq_values[m] = -sum;

        /* Update terms for the next step (they will be needed for future m) */
        for (idx_t j = 0; j <= m - 2; ++j) {
            /* term[j] = term[j] * (m+1) * (m-j-1) / (m-j) */
            seq_t numer = term_values[j] * (seq_t)(m + 1) * (seq_t)(m - j - 1);
            seq_t denom = (seq_t)(m - j);
            term_values[j] = numer / denom;
        }

        /* New term for index m-1 */
        term_values[m - 1] = seq_values[m - 1] * (seq_t)m * (seq_t)(m + 1);
    }

    return seq_values[n];
}
