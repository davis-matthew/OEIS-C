/*
 * A270765  Total sum of the sizes of all blocks with minimal element 2 in all
 *          set partitions of {1,2,...,n}.
 * https://oeis.org/A270765
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n-1} (n-k)·C(n‑2,k‑1)·B_k where B_k is the k‑th
 *            Bell number (B_0 = 1, B_{m+1} = Σ_{i=0}^m C(m,i)·B_i)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- Static tables for Bell numbers and Bell triangle rows ---------- */
#ifdef __cplusplus
static seq_t *Bell = nullptr;
static size_t bell_cap = 0;
static size_t bell_cnt = 0;          /* number of Bell numbers stored (0 … bell_cnt-1) */
static seq_t *row = nullptr;
static seq_t *row_next = nullptr;
static size_t row_cap = 0;
#else
static seq_t *Bell = NULL;
static size_t bell_cap = 0;
static size_t bell_cnt = 0;          /* number of Bell numbers stored (0 … bell_cnt-1) */
static seq_t *row = NULL;
static seq_t *row_next = NULL;
static size_t row_cap = 0;
#endif

/* Ensure the Bell, row and row_next arrays have capacity for indices up to N (i.e. size N+1). */
static void ensure_bell_capacity(idx_t N)
{
    size_t need = (size_t)N + 1;
    if (bell_cap < need) {
        size_t new_cap = bell_cap?bell_cap*2:2;
        while (new_cap < need) new_cap <<= 1;
#ifdef __cplusplus
        seq_t *newBell = new seq_t[new_cap];
        if (Bell) {
            for (size_t i = 0; i < bell_cap; ++i) newBell[i] = Bell[i];
            delete[] Bell;
        }
        Bell = newBell;
#else
        Bell = (seq_t*)realloc(Bell, new_cap * sizeof(seq_t));
#endif
        bell_cap = new_cap;
    }
    if (row_cap < need) {
        size_t new_cap = row_cap?row_cap*2:2;
        while (new_cap < need) new_cap <<= 1;
#ifdef __cplusplus
        seq_t *newRow = new seq_t[new_cap];
        seq_t *newRowNext = new seq_t[new_cap];
        if (row) {
            for (size_t i = 0; i < row_cap; ++i) newRow[i] = row[i];
            delete[] row;
        }
        if (row_next) {
            for (size_t i = 0; i < row_cap; ++i) newRowNext[i] = row_next[i];
            delete[] row_next;
        }
        row = newRow;
        row_next = newRowNext;
#else
        row = (seq_t*)realloc(row, new_cap * sizeof(seq_t));
        row_next = (seq_t*)realloc(row_next, new_cap * sizeof(seq_t));
#endif
        row_cap = new_cap;
    }
}

/* Compute Bell numbers B_0 … B_N using the Bell triangle. */
static void compute_bell_up_to(idx_t N)
{
    if (bell_cnt > N) return;          /* already computed */

    ensure_bell_capacity(N);

    if (bell_cnt == 0) {               /* initialise first row */
        Bell[0] = (seq_t)1;            /* B_0 = 1 */
        row[0] = (seq_t)1;             /* first row of the triangle */
        bell_cnt = 1;                  /* we have B_0 ready */
    }

    for (idx_t n = bell_cnt; n <= N; ++n) {
        /* row currently holds row for n‑1 (length n) */
        /* compute next row of length n+1 in row_next */
        row_next[0] = row[n-1];        /* first element is last of previous row */
        for (idx_t k = 1; k <= n; ++k) {
            row_next[k] = row_next[k-1] + row[k-1];
        }
        Bell[n] = row_next[0];         /* B_n is the first element of the new row */

        /* swap the two rows for next iteration */
        seq_t *tmp = row;
        row = row_next;
        row_next = tmp;
    }
    bell_cnt = N + 1;                  /* Bell[0 … N] are now ready */
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;        /* values below offset are irrelevant */

    /* need Bell numbers up to B_{n-1} */
    compute_bell_up_to(n-1);

    seq_t sum = (seq_t)0;
    idx_t N = n - 2;                   /* upper index for binomial C(N, r) */
    seq_t binom = (seq_t)1;            /* C(N,0) */

    for (idx_t k = 1; k <= n-1; ++k) {
        /* term = binom * (n-k) * B_k */
        seq_t term = binom * (seq_t)(n - k);
        term *= Bell[k];
        sum += term;

        /* update binom to C(N, k) for next iteration */
        if (k < n-1) {
            binom = binom * (seq_t)(N - (k - 1));
            binom = binom / (seq_t)k;
        }
    }
    return sum;
}
