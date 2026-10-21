/*
 * A267482  Triangle of coefficients of Gaussian polynomials [2n+1,1]_q
 *          represented as finite sum of terms (1+q^2)^k*q^(g-k), where k =
 *          0,1,...,g with g=n.
 * https://oeis.org/A267482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 110, i16 n <= 360, i32 n <= 495, i64 n <= 495,
 *            i128 n <= 495, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A267482 – Triangle of coefficients of Gaussian polynomials [2n+1,1]_q.
 *
 *  The triangle rows R(n,x) satisfy
 *      R(-1,x) = -1,   R(0,x) = 1,
 *      R(n,x)  = x*R(n-1,x) - R(n-2,x)   (n ≥ 1).
 *
 *  The coefficient a(n,k) is the x^k coefficient of R(n,x).
 *  The global sequence a(n) is obtained by flattening the rows:
 *      a = [R(0,0), R(1,0),R(1,1), R(2,0),R(2,1),R(2,2), …].
 *
 *  For a global index N, let r be the unique integer with
 *      r(r+1)/2 ≤ N < (r+1)(r+2)/2.
 *  Then
 *      k = N - r(r+1)/2,
 *      a(N) = a(r,k) = coefficient of x^k in R(r,x).
 *
 *  This implementation builds the triangle incrementally the first
 *  time it is needed and reuses the stored rows for any order of
 *  function calls.  All arithmetic uses the built‑in seq_t type.
 *
 *  Complexity:  O(N) in time and O(sqrt(N)) in memory for a query N.
 */

#include <stdlib.h>

static idx_t       computed_rows = 0;          // highest row computed (>=0)
static seq_t *     triangle      = NULL;       // contiguous storage of all rows
static size_t      tri_capacity  = 0;          // allocated storage size (in elements)

static void init_triangle(void) {
    if (triangle != NULL) return;
    tri_capacity = 3;                         // rows 0 and 1 → 1 + 2 = 3 entries
    triangle = (seq_t*)malloc(tri_capacity * sizeof(seq_t));
    if (!triangle) abort();
    triangle[0] = 1;                          // row 0: [1]
    triangle[1] = 1;                          // row 1: [1, 1]
    triangle[2] = 1;
    computed_rows = 1;
}

static void ensure_capacity(size_t needed) {
    if (needed <= tri_capacity) return;
    size_t newcap = tri_capacity ? tri_capacity : 1;
    while (newcap < needed) newcap <<= 1;     // grow geometrically to reduce reallocs
    seq_t *tmp = (seq_t*)realloc(triangle, newcap * sizeof(seq_t));
    if (!tmp) abort();
    triangle = tmp;
    tri_capacity = newcap;
}

/* Compute row n (n ≥ 2) using the recurrence.
 *   row n has n+1 coefficients, stored starting at index n(n+1)/2. */
static void compute_row(idx_t n) {
    size_t start_n    = (size_t)n * (n + 1) / 2;
    size_t start_p1   = (size_t)(n - 1) * n / 2;
    size_t start_p2   = (size_t)(n - 2) * (n - 1) / 2;

    for (size_t k = 0; k <= (size_t)n; ++k) {
        seq_t val = 0;
        if (k > 0) val += triangle[start_p1 + (k - 1)];
        if (k <= (size_t)(n - 2)) val -= triangle[start_p2 + k];
        triangle[start_n + k] = val;
    }
}

/* Return the a(N) value for the global index N. */
seq_t a(idx_t N) {
    /* Determine the triangle row r containing index N. */
    idx_t r = 0;
    while ( ( (__int128)(r + 1) * (r + 2) ) / 2 <= N ) r++;

    idx_t start = ( (__int128)r * (r + 1) ) / 2;
    idx_t k     = N - start;

    /* Lazily initialise and grow the triangle up to row r. */
    init_triangle();
    if (r > computed_rows) {
        /* Ensure storage for rows up to r. */
        for (idx_t n = computed_rows + 1; n <= r; ++n) {
            size_t need = (size_t)(n + 1) * (n + 2) / 2;
            ensure_capacity(need);
            compute_row(n);
        }
        computed_rows = r;
    }

    /* Return the requested coefficient. */
    return triangle[(size_t)start + (size_t)k];
}
