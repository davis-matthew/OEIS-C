/*
 * A278071  Triangle read by rows, coefficients of the polynomials P(n,x) =
 *          (-1)^n*hypergeom( [n,-n], [], x), powers in descending order.
 * https://oeis.org/A278071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 14, i32 n <= 35, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient sequence of the orthogonal polynomials
 * P(n,x) = (-1)^n * hypergeom([n,-n],[],x).  The coefficients are read
 * row by row.  The polynomials satisfy
 *
 *   P_n(x) = (( (4n-2)(2n-3)x + 2 )*P_{n-1}(x) + (2n-1)*P_{n-2}(x))/(2n-3),
 *   for n ≥ 2,  with
 *   P_0(x) = 1    and    P_1(x) = x - 1 .
 *
 * This implementation builds each row on demand, caches the previously
 * computed rows, and returns the required coefficient.  All arithmetic
 * is performed in the type ``seq_t`` (provided by the build system);
 * only the operators +, -, *, /, % and bitwise ops are used as required.
 */

#include <stdlib.h>   /* malloc, realloc */

/* Integer square root for unsigned 64‑bit values. */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = 1ull << 62;
    while (bit) {
        uint64_t nxt = r + bit;
        if ((unsigned __int128)nxt * nxt <= x)
            r = nxt;
        bit >>= 2;
    }
    return r;
}

/* Return the row index that contains global index n.  Rows are 0‑based. */
static inline uint64_t row_of(idx_t n)
{
    /* Solve r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    uint64_t d = 8 * n + 1;
    uint64_t s = isqrt_u64(d);
    uint64_t r = (s - 1) / 2;
    /* Correct any edge effect caused by rounding. */
    while (r * (r + 1) / 2 > n) r--;
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    return r;
}

/* Cache of previously computed rows (arrays of seq_t, descending powers). */
static seq_t **rows = NULL;           /* Pointer array */
static size_t rows_cap   = 0;          /* Allocated capacity */
static size_t rows_used  = 0;          /* Number of rows stored */

/* Ensure that rows[0..r] exist, computing missing ones. */
static void ensure_rows(uint64_t r)
{
    if (rows_used > r) return;

    if (rows_cap == 0) {
        rows_cap = 16;
        rows = (seq_t**)malloc(rows_cap * sizeof(seq_t*));
    } else if (rows_used >= rows_cap) {
        size_t new_cap = rows_cap * 2;
        seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        if (!tmp) return;         /* Out‑of‑memory: silently bail */
        rows = tmp;
        rows_cap = new_cap;
    }

    for (uint64_t i = rows_used; i <= r; ++i) {
        size_t len = i + 1;
        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
        if (!row) return;         /* Out‑of‑memory: silently bail */

        if (i == 0) {
            row[0] = (seq_t)1;
        } else if (i == 1) {
            row[0] = (seq_t)1;        /* coefficient of x^1 */
            row[1] = (seq_t)-1;       /* coefficient of x^0 */
        } else {
            uint64_t ui = (uint64_t)i;
            uint64_t A64 = (4 * ui - 2) * (2 * ui - 3);
            uint64_t B64 = 2;
            uint64_t C64 = 2 * ui - 1;
            uint64_t D64 = 2 * ui - 3;

            seq_t A = (seq_t)A64;
            seq_t B = (seq_t)B64;
            seq_t C = (seq_t)C64;
            seq_t D = (seq_t)D64;

            seq_t *prev1 = rows[i - 1];   /* length i   (degree i-1) */
            seq_t *prev2 = rows[i - 2];   /* length i-1 (degree i-2) */

            seq_t *tmp = (seq_t*)malloc(len * sizeof(seq_t));
            if (!tmp) { free(row); return; }

            /* ((A*(2n-3))*x + B) * P_{i-1} */
            tmp[0] = A * prev1[0];
            for (uint64_t j = 1; j < i; ++j)
                tmp[j] = A * prev1[j] + B * prev1[j - 1];
            tmp[i] = B * prev1[i - 1];

            /* Add C * P_{i-2} shifted by two degrees */
            for (uint64_t t = 0; t < i - 1; ++t)
                tmp[2 + t] = tmp[2 + t] + C * prev2[t];

            /* Divide by D */
            for (uint64_t j = 0; j < len; ++j)
                row[j] = tmp[j] / D;

            free(tmp);
        }

        rows[i] = row;
        rows_used = i + 1;
    }
}

/* Public function required by the benchmark */
seq_t a(idx_t n)
{
    uint64_t r = row_of(n);
    ensure_rows(r);
    uint64_t start = r * (r + 1) / 2;   /* first index of row r */
    uint64_t k = n - start;             /* position within the row */
    return rows[r][k];
}
