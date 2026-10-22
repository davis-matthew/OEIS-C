/*
 * A307879  Expansion of e.g.f. exp(4*x)*(sec(x) + tan(x)).
 * https://oeis.org/A307879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0}^{n} binom(n,k) · 4^k · E_{n-k} where E_m are the
 *            Euler zigzag numbers (OEIS A000111)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ----- Global state -------------------------------------------------- */
static seq_t *E        = NULL;      /* Euler numbers E[0..] */
static size_t E_len    = 0;          /* number of Euler entries computed */

static seq_t *prev_row = NULL;      /* previous row of Entringer triangle */
static seq_t *cur_row  = NULL;      /* current  row of Entringer triangle */
static size_t row_cap  = 0;          /* allocated size of row buffers */

/* ----- Euler numbers ------------------------------------------------- */
static void compute_euler_upto(idx_t max_n)
{
    /* Return if we already have enough data. */
    if (max_n + 1 <= E_len) return;

    /* Allocate or grow the Euler array. */
    if (E == NULL) {
        E = (seq_t *)malloc((max_n + 1) * sizeof(seq_t));
        if (!E) return;       /* silent failure */
        E[0] = (seq_t)1;
        E_len = 1;
    } else if (max_n + 1 > E_len) {
        seq_t *tmp = (seq_t *)realloc(E, (max_n + 1) * sizeof(seq_t));
        if (!tmp) return;
        E = tmp;
    }

    /* Allocate or enlarge the row buffers. */
    if (row_cap < (size_t)(max_n + 1)) {
        seq_t *tmp;

        tmp = (seq_t *)realloc(prev_row, (max_n + 1) * sizeof(seq_t));
        if (!tmp) return;
        prev_row = tmp;

        tmp = (seq_t *)realloc(cur_row, (max_n + 1) * sizeof(seq_t));
        if (!tmp) return;
        cur_row = tmp;

        row_cap = max_n + 1;

        /* First allocation: initialise the row for n = 0. */
        if (E_len == 0)
            prev_row[0] = (seq_t)1;
    }

    /* Ensure the row for n = 0 is correct if we just allocated info. */
    if (E_len == 1 && prev_row[0] == (seq_t)0)
        prev_row[0] = (seq_t)1;

    /* Compute rows from the current length up to max_n. */
    for (idx_t n = (idx_t)E_len; n <= max_n; ++n) {
        /* E(n,0) = 0 for n ≥ 1 */
        cur_row[0] = (seq_t)0;

        for (idx_t k = 1; k <= n; ++k) {
            cur_row[k] = cur_row[k - 1] + prev_row[n - k];
        }

        E[(size_t)n] = cur_row[(size_t)n];

        /* Rotate buffers for next iteration. */
        seq_t *tmp = prev_row;
        prev_row = cur_row;
        cur_row  = tmp;
    }

    E_len = (size_t)max_n + 1;
}

/* ----- Target sequence ----------------------------------------------- */
static seq_t *A        = NULL;      /* sequence values a[0..] */
static size_t A_len    = 0;          /* number of a entries computed */

/* Compute a[0..max_n] */
static void compute_sequence_upto(idx_t max_n)
{
    if (max_n + 1 <= A_len) return;    /* already computed */

    /* Allocate or extend the sequence array. */
    if (A == NULL) {
        A = (seq_t *)malloc((max_n + 1) * sizeof(seq_t));
        if (!A) return;
    } else if (max_n + 1 > A_len) {
        seq_t *tmp = (seq_t *)realloc(A, (max_n + 1) * sizeof(seq_t));
        if (!tmp) return;
        A = tmp;
    }

    /* Ensure Euler numbers are available. */
    compute_euler_upto(max_n);

    /* Compute new terms. */
    for (idx_t n = (idx_t)A_len; n <= max_n; ++n) {
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1; /* C(n,0) */
        seq_t pow4  = (seq_t)1; /* 4^0 */

        for (idx_t k = 0; k <= n; ++k) {
            seq_t term = binom * pow4 * E[(size_t)(n - k)];
            sum += term;

            if (k < n) { /* prepare next binomial coefficient and 4^k */
                binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
                pow4  = pow4 * (seq_t)4;
            }
        }

        A[(size_t)n] = sum;
    }

    A_len = (size_t)max_n + 1;
}

/* ----- Public function ----------------------------------------------- */
seq_t a(idx_t n)
{
    compute_sequence_upto(n);
    return A[(size_t)n];
}
