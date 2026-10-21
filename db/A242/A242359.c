/*
 * A242359  Irregular triangular array of denominators of the positive
 *          rational numbers ordered as in Comments.
 * https://oeis.org/A242359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf, frac
 * Formula:   a(n) = denominator of the n-th rational in the irregular
 *            triangle described in Comments. The triangle is built
 *            row‑by‑row: • Row 1: 1/1 • Row 2: 2/1 • For n ≥ 3: start with
 *            (x+1) for every x in row n-1 and (1/(1+x)) for every x in row
 *            n-2; sort the results in decreasing order. All fractions are
 *            s...
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

typedef struct { seq_t num; seq_t den; } frac_t;
typedef struct { size_t len; frac_t *data; } row_t;

#define MAX_ROWS 120
#define FIB_SIZE 200

static row_t rows[MAX_ROWS + 1];
static idx_t fib[FIB_SIZE];
static int fib_computed = 0;

/* Compute Fibonacci numbers up to FIB_SIZE.  They are used only for
   indexing and are of type idx_t, so 64‑bit arithmetic suffices. */
static void compute_fib(void) {
    if (fib_computed) return;
    fib[0] = 0;
    fib[1] = 1;
    fib[2] = 1;
    for (int i = 3; i < FIB_SIZE; ++i) fib[i] = fib[i - 1] + fib[i - 2];
    fib_computed = 1;
}

/* Ensure that row r has been built.  Rows are built incrementally
   and stored in the static array 'rows'. */
static void ensure_row(int r) {
    if (rows[r].data != NULL) return;          /* already built */

    if (r == 1) {
        rows[r].len = 1;
        rows[r].data = (frac_t *)malloc(sizeof(frac_t));
        rows[r].data[0].num = 1;
        rows[r].data[0].den = 1;
        return;
    }

    if (r == 2) {
        rows[r].len = 1;
        rows[r].data = (frac_t *)malloc(sizeof(frac_t));
        rows[r].data[0].num = 2;
        rows[r].data[0].den = 1;
        return;
    }

    /* Build previous two rows first */
    ensure_row(r - 1);
    ensure_row(r - 2);

    size_t lenA = rows[r - 1].len;
    size_t lenB = rows[r - 2].len;
    size_t lenC = lenA + lenB;

    rows[r].len = lenC;
    rows[r].data = (frac_t *)malloc(lenC * sizeof(frac_t));

    size_t ia = 0;                /* index into row r-1          */
    size_t ib = lenB;             /* index into row r-2 (backward) */
    size_t k  = 0;                /* output index                */

    /* Merge the two sorted lists:
       * A: (x+1) from row r-1 (already sorted descending)
       * B: (1/(1+x)) from row r-2 (sorted ascending, so we traverse from the end) */
    while (ia < lenA && ib > 0) {
        seq_t numA = rows[r - 1].data[ia].num + rows[r - 1].data[ia].den;
        seq_t denA = rows[r - 1].data[ia].den;
        seq_t numB = rows[r - 2].data[ib - 1].den;
        seq_t denB = rows[r - 2].data[ib - 1].num + rows[r - 2].data[ib - 1].den;

        /* Compare numA/denA with numB/denB via cross multiplication.
           The values are small enough that standard multiplication
           does not overflow for the range of n considered. */
        if (numA * denB > numB * denA) {
            rows[r].data[k++] = (frac_t){numA, denA};
            ++ia;
        } else {
            rows[r].data[k++] = (frac_t){numB, denB};
            --ib;
        }
    }

    /* Copy any remaining elements from the first list */
    while (ia < lenA) {
        seq_t numA = rows[r - 1].data[ia].num + rows[r - 1].data[ia].den;
        seq_t denA = rows[r - 1].data[ia].den;
        rows[r].data[k++] = (frac_t){numA, denA};
        ++ia;
    }

    /* Copy the remaining elements from the second list */
    while (ib > 0) {
        seq_t numB = rows[r - 2].data[ib - 1].den;
        seq_t denB = rows[r - 2].data[ib - 1].num + rows[r - 2].data[ib - 1].den;
        rows[r].data[k++] = (frac_t){numB, denB};
        --ib;
    }
}

/* Main function that returns a(n).  For n ≥ 1 it gives the denominator
   of the n-th rational in the triangle. */
seq_t a(idx_t n) {
    if (n == 0) return 0;        /* undefined for zero, but guard */

    compute_fib();

    /* Find the row r that contains index n.
       The cumulative count up to row r is F(r+2)-1 (F = Fibonacci). */
    int r = 1;
    while (fib[r + 2] - 1 < n) ++r;

    ensure_row(r);

    /* Position of n within that row (1‑based). */
    idx_t prev_sum = fib[r + 1] - 1;    /* sum up to row r‑1 */
    idx_t pos = n - prev_sum;           /* 1‑based index inside row r */

    return rows[r].data[(size_t)(pos - 1)].den;
}
