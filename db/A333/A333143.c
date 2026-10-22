/*
 * A333143  Triangle read by rows: T(n, k) = qStirling2(n, k, q) for q = 3,
 *          with 0 <= k <= n.
 * https://oeis.org/A333143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term of the triangle T(n,k)=qStirling2(n,k,3)
 *            read row–by–row. T(n,k) satisfies
 *            T(n,k)=T(n-1,k-1)+qBracket(k+1)*T(n-1,k) (k>0), with T(n,0)=1
 *            and T(0,k)=0^k. qBracket(m)=1+3+⋯+3^{m-1}
 * Range:     u8 n <= 15, u16 n <= 23, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **rows = NULL;          /* rows[i] points to array of length i+1 */
static size_t rows_cap = 0;          /* allocated capacity for rows array   */
static size_t rows_count = 0;        /* number of rows actually allocated  */

static seq_t *bracket = NULL;       /* bracket[j] = sum_{t=0}^{j-1} 3^t   */
static size_t bracket_cap = 0;      /* allocated capacity for bracket array */

/* Ensure the bracket array can address indices up to need.  */
static void ensure_brackets(size_t need)
{
    /* if already sufficient return */
    if (need < bracket_cap) return;

    /* grow until we can hold all required indices */
    size_t new_cap = bracket_cap == 0 ? 2 : bracket_cap * 2;
    if (new_cap <= need) new_cap = need + 1;

    seq_t *tmp = (seq_t *)realloc(bracket, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                 /* fatal allocation failure */

    if (bracket_cap == 0) {            /* first time: initialise */
        tmp[0] = (seq_t)0;
        tmp[1] = (seq_t)1;
        bracket_cap = 2;
    }

    for (size_t j = bracket_cap; j < new_cap; ++j)
        tmp[j] = tmp[j - 1] * (seq_t)3 + (seq_t)1;

    bracket_cap = new_cap;
    bracket = tmp;
}

/* Allocate and fill rows 0..target (inclusive).  */
static void ensure_rows(size_t target)
{
    if (target < rows_count) return;

    /* grow the rows array if needed */
    if (rows_cap <= target) {
        size_t new_cap = rows_cap == 0 ? target + 1 : rows_cap * 2;
        if (new_cap <= target) new_cap = target + 1;

        seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        if (!tmp) exit(1);                      /* allocation failure */
        rows = tmp;
        rows_cap = new_cap;
    }

    /* Create each new row */
    for (size_t i = rows_count; i <= target; ++i) {
        rows[i] = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        if (!rows[i]) exit(1);                /* allocation failure */

        /* First element of every row */
        rows[i][0] = (seq_t)1;

        if (i == 0) continue;                 /* first row trivially complete */

        /* Ensure brackets up to index i (needed for k+1) */
        ensure_brackets(i + 1);

        seq_t *prev = rows[i - 1];
        /* Compute inner elements (k=1 .. i-1) */
        for (size_t k = 1; k < i; ++k)
            rows[i][k] = prev[k - 1] + bracket[k + 1] * prev[k];

        /* Last element of the row (k = i) */
        rows[i][i] = prev[i - 1];
    }

    rows_count = target + 1;
}

/* Return the n-th term of the sequence.  */
seq_t a(idx_t n)
{
    /* Determine the row containing index n.  */
    size_t rn = 0;
    size_t cumulative = 1;               /* sum of lengths up to current row */
    while (cumulative <= (size_t)n) {
        ++rn;
        cumulative += rn + 1;
    }

    /* Ensure we have computed all rows up to rn. */
    ensure_rows(rn);

    /* Compute column within that row. */
    size_t prev_sum = (rn * (rn + 1)) / 2;
    size_t col = (size_t)n - prev_sum;

    return rows[rn][col];
}
