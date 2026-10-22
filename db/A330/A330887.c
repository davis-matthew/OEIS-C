/*
 * A330887  Irregular triangle read by rows: T(n,k) is the number of
 *          partitions of n into k consecutive parts that differ by 3, n >= 1,
 *          k >= 1, and the first element of column k is in the row that is
 *          the k-th pentagonal number (A000326).
 * https://oeis.org/A330887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 1 if ∃ k ≥ 1 such that n ≥ P(k)=k*(3k‑1)/2 and (n‑P(k))
 *            mod k == 0, otherwise 0. Here P(k) is the k‑th pentagonal
 *            number. The sequence is read by rows of the irregular triangle
 *            T(n,k)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t max_row      = 0;     /* highest row that has been pre‑computed */
static idx_t cur_k        = 0;     /* largest k with P(k) ≤ current row   */
static idx_t next_k       = 1;     /* next k to be checked for P(k)       */
static idx_t next_pent    = 1;     /* P(next_k)                           */

static idx_t *K            = NULL; /* K[row] = maximal k with P(k) ≤ row  */
static seq_t *C            = NULL; /* C[row] = cumulative number of entries up to row */
static idx_t capacity      = 0;    /* allocated size for K and C           */

/* ------------------------------------------------------------------ */
/* Ensure that rows 0 … target are pre‑computed.  Resize arrays if
   necessary.                                                         */
static void ensure_rows(idx_t target)
{
    /* Grow the arrays if the requested target exceeds current capacity. */
    if (capacity <= target) {
        idx_t new_cap = capacity ? capacity << 1 : 8;
        while (new_cap <= target) new_cap <<= 1;
        idx_t *newK = (idx_t *)realloc(K, new_cap * sizeof(idx_t));
        seq_t *newC = (seq_t *)realloc(C, new_cap * sizeof(seq_t));
        if (!newK || !newC) abort();
        K = newK;
        C = newC;
        capacity = new_cap;
    }

    /* Initialise entry 0 if this is the first run. */
    if (max_row == 0) {
        K[0] = 0;
        C[0] = 0;
    }

    /* Compute all missing rows up to 'target'. */
    for (idx_t r = max_row + 1; r <= target; ++r) {
        /* Advance cur_k until P(cur_k) ≤ r. */
        while (next_pent <= r) {
            ++cur_k;
            ++next_k;
            next_pent = next_k * ((3 * (idx_t)next_k) - 1) / 2;
        }
        K[r] = cur_k;
        C[r] = C[r - 1] + (seq_t)K[r];
    }
    max_row = target;
}

/* ------------------------------------------------------------------ */
/* Return 1 if the entry T(row,col) is 1, else 0. */
static inline seq_t triangle_entry(idx_t row, idx_t col)
{
    seq_t row_s   = (seq_t)row;
    seq_t denom   = (seq_t)3 * (seq_t)col * ((seq_t)col - 1) / 2;
    if (row_s <= denom) return 0;
    seq_t diff = row_s - denom;
    return (diff % (seq_t)col == 0) ? 1 : 0;
}

/* ------------------------------------------------------------------ */
/* Main function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* a(n) is defined only for n ≥ 1 */

    /* Ensure that C[max_row] ≥ n by generating rows as needed. */
    while (max_row == 0 || C[max_row] < (seq_t)n)
        ensure_rows(max_row + 1);

    /* Binary search for the row that contains the n‑th entry. */
    idx_t lo = 1, hi = max_row;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (C[mid] >= (seq_t)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t row = lo;

    seq_t before = C[row - 1];
    seq_t diff   = (seq_t)n - before;
    idx_t col    = (idx_t)diff;   /* 1 ≤ col ≤ K[row] */

    return triangle_entry(row, col);
}
