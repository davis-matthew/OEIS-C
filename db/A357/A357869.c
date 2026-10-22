/*
 * A357869  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=0..n} (k*j)!*
 *          Stirling2(n,k*j)/j!.
 * https://oeis.org/A357869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the term read by antidiagonals from the square array
 *            T(n,k) defined by T(0,k) = 1 and, for n>0, T(n,k) = Σ_{j=0..n}
 *            (k·j)! · Stirling2(n, k·j) / j! (Stirling2 is the Stirling
 *            number of the second kind; terms with k·j>n contribute 0). The
 *            mapping from the linear index n to the coordinates...
 * Range:     u8 n <= 41, u16 n <= 73, u32 n <= 132, u64 n <= 319,
 *            u128 n <= 735, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Memoised factorials ---------- */
static seq_t *factorial_arr = NULL;
static size_t factorial_cap = 0;   /* number of factorials available (size) */

/* ensure factorial_arr has factorials up to n (inclusive) */
static void ensure_factorial(size_t n)
{
    if (n < factorial_cap) return;

    factorial_arr = (seq_t*)realloc(factorial_arr, (n + 1) * sizeof(seq_t));
    if (!factorial_arr) exit(EXIT_FAILURE);

    if (factorial_cap == 0) {
        factorial_arr[0] = (seq_t)1;
        factorial_cap   = 1;
    }
    for (size_t i = factorial_cap; i <= n; ++i) {
        factorial_arr[i] = factorial_arr[i-1] * (seq_t)i;
    }
    factorial_cap = n + 1;
}

/* ---------- Memoised Stirling numbers of the second kind ---------- */
static seq_t **stirling = NULL;       /* stirling[n][k] = S(n,k) */
static size_t stirling_rows = 0;      /* number of rows stored */

/* ensure all rows up to n are present */
static void ensure_stirling(size_t n)
{
    if (n < stirling_rows) return;

    size_t old_rows = stirling_rows;
    stirling = (seq_t**)realloc(stirling, (n + 1) * sizeof(seq_t*));
    if (!stirling) exit(EXIT_FAILURE);

    for (size_t i = old_rows; i <= n; ++i) {
        stirling[i] = (seq_t*)calloc(i + 1, sizeof(seq_t));
        if (!stirling[i]) exit(EXIT_FAILURE);
    }

    /* compute rows 0..n */
    for (size_t r = 0; r <= n; ++r) {
        seq_t *row = stirling[r];
        for (size_t m = 0; m <= r; ++m) {
            if (r == 0 && m == 0) {
                row[0] = (seq_t)1;
            } else {
                seq_t s1 = (m > 0 && r > 0) ? stirling[r-1][m-1] : (seq_t)0;
                seq_t s2 = (m <= r-1) ? stirling[r-1][m] : (seq_t)0;
                row[m] = s1 + s2 * (seq_t)m;
            }
        }
    }
    stirling_rows = n + 1;
}

/* ---------- Antidiagonal coordinate helpers ---------- */
/* Return the largest m such that m·(m+1)/2 <= idx. */
static idx_t diag_of(idx_t idx)
{
    idx_t lo = 0;
    idx_t hi = (idx < UINT64_MAX) ? idx + 1 : idx;   /* guarantee hi > answer */

    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= idx)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;    /* the desired m */
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* 1. Map n to array coordinates (row, col) */
    idx_t m     = diag_of(n);
    idx_t start = (idx_t)(((__uint128_t)m * (m + 1)) / 2);
    idx_t row   = n - start;        /* i */
    idx_t col   = m - row;          /* k */

    /* 2. Prepare factorials up to needed sizes */
    size_t max_fact = (size_t)row;   /* for j! */
    if (col > 0) {
        size_t tmp = (size_t)col * (size_t)row;
        if (tmp > max_fact) max_fact = tmp;
    }
    ensure_factorial(max_fact);

    /* 3. Ensure Stirling numbers up to row are present */
    ensure_stirling((size_t)row);

    /* 4. Compute T(row,col) = Σ_{j=0..row} (col*j)! * S(row,col*j)/j! */
    seq_t sum = (seq_t)0;

    for (idx_t j = 0; j <= row; ++j) {
        idx_t kj = col * j;     /* k·j */
        if (kj > row) break;    /* S(row,kj)=0 beyond this */

        seq_t ratio = factorial_arr[(size_t)kj] / factorial_arr[(size_t)j];
        seq_t term  = ratio * stirling[(size_t)row][(size_t)kj];
        sum += term;
    }
    return sum;
}
