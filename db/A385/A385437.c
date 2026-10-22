/*
 * A385437  Triangle read by rows: T(n,k) is the number of proper vertex
 *          colorings of the n-complete bipartite graph with a perfect
 *          matching removed using exactly k interchangeable colors, for n >=
 *          1 and 2 <= k <= 2n.
 * https://oeis.org/A385437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   the sequence is the triangle read by rows: T(n,k) = Σ_{s=0..n}
 *            C(n,s) * Σ_{j=0..k-n+s} S₂(s,j) * S₂(s,k-n+s-j) where S₂(s,j) is
 *            the Stirling number of the second kind. The a(n) sequence lists
 *            the terms T(n,k) with 2 ≤ k ≤ 2n in row‑major order
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* seq_t and idx_t are provided by the build system. */
typedef uint64_t idx_t;   /* assumed definition */

/* --- Stirling numbers of the second kind, S[s][j] for 0 ≤ s,j ≤ max_s --- */
static seq_t **S = NULL;          /* array of pointers to rows */
static idx_t max_s = 0;           /* highest row computed so far */

/* initialise the table with row 0 */
static void init_stirling(void)
{
    S = (seq_t **)malloc(sizeof(seq_t *));
    if (!S) exit(EXIT_FAILURE);
    S[0] = (seq_t *)malloc(sizeof(seq_t));
    if (!S[0]) exit(EXIT_FAILURE);
    S[0][0] = 1;   /* S(0,0) = 1 */
    max_s = 0;
}

/* extend the table up to row 'need' inclusive */
static void ensure_stirling(idx_t need)
{
    if (!S) init_stirling();
    if (max_s >= need) return;

    for (idx_t s = max_s + 1; s <= need; ++s) {
        /* allocate the new row */
        seq_t *row = (seq_t *)malloc((s + 1) * sizeof(seq_t));
        if (!row) exit(EXIT_FAILURE);
        row[0] = 0;              /* S(s,0) = 0 for s>0 */

        seq_t *prev = S[s - 1];
        for (idx_t j = 1; j <= s; ++j) {
            seq_t term1 = (j <= s - 1) ? (prev[j] * j) : 0;
            seq_t term2 = prev[j - 1];
            row[j] = term1 + term2;
        }

        /* enlarge the pointer array */
        S = (seq_t **)realloc(S, (s + 1) * sizeof(seq_t *));
        if (!S) exit(EXIT_FAILURE);
        S[s] = row;
        max_s = s;
    }
}

/* binomial coefficient C(n,k) computed multiplicatively */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - i + 1);
        res = res / i;          /* exact division */
    }
    return res;
}

/* compute T(n,k) using the precomputed Stirling numbers */
static seq_t compute_T(idx_t n, idx_t k)
{
    ensure_stirling(n);
    seq_t total = 0;

    for (idx_t s = 0; s <= n; ++s) {
        /* w = k - n + s; we only contribute if w >= 0 and w <= 2s */
        int64_t w = (int64_t)k - (int64_t)n + (int64_t)s;
        if (w < 0 || w > 2 * (int64_t)s) continue;

        /* j ranges from max(0, w-s) to min(s, w)   */
        int64_t j_low  = (w > (int64_t)s) ? (w - (int64_t)s) : 0;
        int64_t j_high = (w < (int64_t)s) ? w : s;

        seq_t inner = 0;
        seq_t *row = S[s];
        for (int64_t j = j_low; j <= j_high; ++j) {
            seq_t a = row[j];
            seq_t b = row[w - j];
            inner += a * b;
        }

        total += binom(n, s) * inner;
    }
    return total;
}

/* integer ceil of the square root using long double for safety */
static idx_t ceil_sqrt(idx_t val)
{
    if (val == 0) return 0;
    long double d = sqrt((long double)val);
    idx_t m = (idx_t)d;
    /* adjust for possible rounding errors */
    while ((m + 1) * (m + 1) <= val) ++m;
    while (m * m > val) --m;
    if (m * m < val) ++m;
    return m;
}

/* ------------------------------------------------------------------------- */
/* The required public function                                            */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* safety for out-of-range calls */

    idx_t row   = ceil_sqrt(n);          /* determine the row number  */
    idx_t prev  = (row - 1) * (row - 1); /* terms before this row      */
    idx_t pos   = n - prev;              /* 1‑based position in row    */
    idx_t k     = pos + 1;               /* column number (2 ≤ k ≤ 2*row) */

    return compute_T(row, k);
}
