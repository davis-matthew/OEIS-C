/*
 * A365529  a(n) = Sum_{k=0..floor((n-1)/5)} Stirling2(n,5*k+1).
 * https://oeis.org/A365529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/5)} Stirling2(n,5*k+1) Implements
 *            an iterative computation of Stirling numbers of the second kind
 *            and sums the terms with index congruent to 1 modulo 5
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#ifdef __cplusplus
#include <vector>
#include <utility>
#endif

/* Global state ------------------------------------------------------------ */
static idx_t max_computed = 0;   /* largest n for which a(n) has been computed */

#ifdef __cplusplus
static std::vector<seq_t> A;        /* a(n) values                 */
static std::vector<seq_t> prev_row; /* Stirling numbers for row n-1 */
static std::vector<seq_t> curr_row; /* Stirling numbers for row n   */

static void ensure_capacity(idx_t n)
{
    idx_t need = n + 1;
    if (A.size() < need) {
        A.resize(need);
        prev_row.resize(need);
        curr_row.resize(need);
        /* all entries are value‑initialised to 0 in C++ */
    }
}

static void maybe_init(void)
{
    if (max_computed == 0 && A.empty()) {
        ensure_capacity(0);
        prev_row[0] = 1;      /* row 0: Stirling(0,0) = 1 */
        A[0] = 0;             /* a(0) = 0                 */
        max_computed = 0;
    }
}
#else
static seq_t *A = NULL;
static seq_t *prev_row = NULL;
static seq_t *curr_row = NULL;
static size_t capacity = 0;    /* allocated size for the three arrays */

static void ensure_capacity(idx_t n)
{
    size_t need = n + 1;
    if (capacity < need) {
        size_t old = capacity;
        capacity = need;
        A        = (seq_t *)realloc(A, capacity * sizeof(seq_t));
        prev_row = (seq_t *)realloc(prev_row, capacity * sizeof(seq_t));
        curr_row = (seq_t *)realloc(curr_row, capacity * sizeof(seq_t));
        /* Zero‑initialise new elements */
        for (size_t i = old; i < capacity; ++i) {
            A[i]        = 0;
            prev_row[i] = 0;
            curr_row[i] = 0;
        }
    }
}

static void maybe_init(void)
{
    if (max_computed == 0 && A == NULL) {
        ensure_capacity(0);
        prev_row[0] = 1;  /* row 0: Stirling(0,0) = 1 */
        A[0]        = 0;  /* a(0) = 0               */
        max_computed = 0;
    }
}
#endif

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    maybe_init();

    if (n <= max_computed)
        return A[n];

    /* Ensure arrays are large enough */
    ensure_capacity(n);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
#ifdef __cplusplus
        curr_row[0] = 0;
        for (idx_t k = 1; k <= i; ++k) {
            curr_row[k] = prev_row[k] * (seq_t)k + prev_row[k - 1];
        }
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; k += 5) sum += curr_row[k];
        A[i] = sum;
        std::swap(prev_row, curr_row);   /* next iteration uses the new row */
#else
        curr_row[0] = 0;
        for (idx_t k = 1; k <= i; ++k) {
            curr_row[k] = prev_row[k] * (seq_t)k + prev_row[k - 1];
        }
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; k += 5) sum += curr_row[k];
        A[i] = sum;
        /* swap pointers for the next iteration */
        seq_t *tmp = prev_row;
        prev_row = curr_row;
        curr_row = tmp;
#endif
    }

    max_computed = n;
    return A[n];
}
