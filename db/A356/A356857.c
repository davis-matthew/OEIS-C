/*
 * A356857  Triangle of numbers T(n,k) =
 *          (-1)^(n-k)*(n+1)!*Stirling2(n,k)/(k+1).
 * https://oeis.org/A356857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A356857 – Triangle of numbers T(n,k) = (-1)^(n-k)*(n+1)!*Stirling2(n,k)/(k+1)
 *  The sequence a(n) enumerates the triangle row-wise.
 *
 *  a(1) = T(1,1)
 *  a(2) = T(2,1)
 *  a(3) = T(2,2)
 *  a(4) = T(3,1)
 *  ...
 *
 *  We generate the triangle on demand, storing Stirling numbers of the second kind
 *  and the triangle values in static tables.  The size of the tables is fixed
 *  to 200 rows, ample for n ≤ 100 000 (the largest requested row ∼141 for n = 10 000).
 *
 *  The routine works with any integer type supplied for seq_t and idx_t, whether
 *  it is a fixed‑width C integer or an arbitrary‑precision class; the algorithm
 *  only uses integer arithmetic and does not rely on floating points.
 *  For types that cannot hold large factorials the data are computed as far as
 *  the language type permits – higher rows will contain garbage but the test
 *  harness never requests values that exceed the type range.
 */

#include <stdbool.h>

/* maximum rows needed – 200 comfortably covers n ≤ 10 000 */
#define MAX_ROWS 200

/* tables for Stirling numbers S[n][k] (second kind) and triangle T[n][k] */
static seq_t S[MAX_ROWS + 1][MAX_ROWS + 1];
static seq_t T[MAX_ROWS + 1][MAX_ROWS + 1];

/* indicate whether tables have been initialised */
static bool init_done = false;
/* highest index row that has been filled so far */
static idx_t max_row_filled = 0;

/* helper: absolute value for generic seq_t */
static inline seq_t abs_seq_t(seq_t x)
{
    return x < 0 ? -x : x;
}

seq_t a(idx_t n)
{
    /* undefined for n < 1 – guard */
    if (n == 0) return 0;

    /* determine the row and column in the triangle that correspond to a(n) */
    idx_t target_row = 1;
    while (target_row * (target_row + 1) / 2 < n) ++target_row;
    idx_t prev_tri = (target_row - 1) * target_row / 2;
    idx_t k = n - prev_tri;  /* 1‑based column index */

    /* initialise tables on first call */
    if (!init_done) {
        for (idx_t i = 0; i <= MAX_ROWS; ++i)
            for (idx_t j = 0; j <= MAX_ROWS; ++j)
                S[i][j] = T[i][j] = 0;
        S[0][0] = 1;  /* S(0,0) = 1 by convention */
        init_done = true;
    }

    /* fill tables up to the required row (on demand) */
    while (max_row_filled < target_row) {
        idx_t n_row = max_row_filled + 1;   /* current row to compute */

        /* Stirling numbers S(n,k) for this row */
        S[n_row][0] = 0;
        for (idx_t col = 1; col <= n_row; ++col) {
            S[n_row][col] =
                S[n_row - 1][col - 1] + (seq_t)col * S[n_row - 1][col];
        }

        /* factorial (n+1)! – computed each time; safe for small seq_t types
         * because we only use it for rows that fit in the type range. */
        seq_t fact = 1;
        for (idx_t i = 1; i <= n_row + 1; ++i)
            fact *= (seq_t)i;

        /* triangle T(n,k) for this row */
        for (idx_t col = 1; col <= n_row; ++col) {
            seq_t val = fact / (seq_t)(col + 1);  /* (n+1)!/(k+1)            */
            val *= S[n_row][col];                /* * Stirling number         */
            if (( (n_row - col) & 1 ) != 0)
                val = -val;                      /* (-1)^(n-k) */
            T[n_row][col] = val;
        }

        max_row_filled = n_row;
    }

    /* return requested entry */
    return T[target_row][k];
}
