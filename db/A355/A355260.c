/*
 * A355260  Triangle read by rows, T(n, k) = Bell(k) * |Stirling1(n, k)|.
 * https://oeis.org/A355260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n,k) with T(n,k) = Bell(k) * |Stirling1(n,k)| Triangle
 *            read by rows (row n has n+1 entries). The sequence a(0), a(1), …
 *            is the concatenation of all rows. All arithmetic is performed
 *            with the compile‑time types seq_t (arbitrary precision or
 *            unsigned integer) and idx_t (uint64_t)
 * Range:     u8 n <= 22, u16 n <= 38, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_ROWS 200          /* covers n up to ≈10000 (row ~140) */

/* ---------- Tables for Bell numbers and unsigned Stirling numbers --------- */
static seq_t bell[MAX_ROWS + 1];
static size_t bell_len = 0;      /* number of Bell values computed (max index + 1) */

static seq_t stirling[MAX_ROWS + 1][MAX_ROWS + 1];
static size_t stirling_rows = 0; /* number of Stirling rows computed (max row + 1) */

/* ---------- Forward declarations ---------- */
static seq_t binom(idx_t n, idx_t k);
static void ensure_bell(idx_t up_to);
static void ensure_stirling(idx_t up_to);
static void index_to_rc(idx_t idx, idx_t *row, idx_t *col);

/* ---------- Utility: binomial coefficient (exact integer arithmetic) ---------- */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* ---------- Ensure Bell numbers up to `up_to` are computed ---------- */
static void ensure_bell(idx_t up_to)
{
    if (bell_len == 0) {              /* initialise */
        bell[0] = (seq_t)1;
        bell_len = 1;
    }
    if (up_to < bell_len) return;

    for (idx_t m = bell_len; m <= up_to; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= m - 1; ++k) {
            sum += binom(m - 1, k) * bell[k];
        }
        bell[m] = sum;
    }
    bell_len = up_to + 1;
}

/* ---------- Ensure Stirling numbers of the first kind up to row `up_to` ---------- */
static void ensure_stirling(idx_t up_to)
{
    if (stirling_rows == 0) {         /* initialise */
        for (idx_t i = 0; i <= MAX_ROWS; ++i)
            for (idx_t j = 0; j <= MAX_ROWS; ++j)
                stirling[i][j] = (seq_t)0;
        stirling[0][0] = (seq_t)1;
        stirling_rows = 1;
    }
    if (up_to < stirling_rows) return;

    for (idx_t n = stirling_rows; n <= up_to; ++n) {
        stirling[n][0] = (seq_t)0;
        for (idx_t k = 1; k <= n; ++k) {
            /* s(n,k) = s(n-1,k-1) + (n-1)*s(n-1,k) */
            seq_t term1 = stirling[n - 1][k - 1];
            seq_t term2 = stirling[n - 1][k] * (seq_t)(n - 1);
            stirling[n][k] = term1 + term2;
        }
    }
    stirling_rows = up_to + 1;
}

/* ---------- Convert flattened index to (row, column) --------------------- */
static void index_to_rc(idx_t idx, idx_t *row, idx_t *col)
{
    /* Brute‑force small loops: r^2 ≈ 2*idx, so at most ~150 iterations */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= idx) ++r;
    *row = r;
    *col = idx - r * (r + 1) / 2;
}

/* ---------- Public sequence function -------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    idx_t row, col;
    index_to_rc(n, &row, &col);

    /* Guard max size (should not be needed for given limits) */
    if (row > MAX_ROWS || col > MAX_ROWS) return (seq_t)0;

    ensure_stirling(row);
    ensure_bell(col);

    seq_t stir = stirling[row][col];
    seq_t res  = bell[col] * stir;  /* all ops are defined on seq_t */
    return res;
}
