/*
 * A290452  Triangle formed by reading the triangle of Eulerian numbers
 *          (A173018) mod 2.
 * https://oeis.org/A290452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Eulerian(n_row, k) mod 2 where n = n_row*(n_row+1)/2 + k,
 *            0 ≤ k ≤ n_row Recurrence (mod 2): A[0][0] = 1 A[r][k] =
 *            ((k+1)&1)*A[r-1][k] ^ ((r-k)&1)*A[r-1][k-1] (0 ≤ k ≤ r)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* internal storage of the triangle: each cell contains 0 or 1 */
static unsigned char **tri = NULL;   /* rows 0..max_row */
static size_t max_row = 0;

/* build rows 0..target of the Eulerian triangle modulo 2 */
static void
build_triangle_up_to(size_t target)
{
    if (tri == NULL) { /* initialise first row */
        tri = (unsigned char**)malloc(sizeof(unsigned char*));
        tri[0] = (unsigned char*)malloc(sizeof(unsigned char));
        tri[0][0] = 1;
        max_row = 0;
    }
    if (max_row >= target)
        return;

    tri = (unsigned char**)realloc(tri, (target + 1) * sizeof(unsigned char*));
    for (size_t r = max_row + 1; r <= target; ++r) {
        tri[r] = (unsigned char*)malloc((r + 1) * sizeof(unsigned char));
        for (size_t k = 0; k <= r; ++k) {
            unsigned char val = 0;
            /* term from A[r-1][k] */
            if (k <= r - 1) {
                unsigned char coeff = ((k + 1) & 1);
                if (coeff)
                    val ^= tri[r - 1][k];
            }
            /* term from A[r-1][k-1] */
            if (k > 0) {
                unsigned char coeff = ((r - k) & 1);
                if (coeff)
                    val ^= tri[r - 1][k - 1];
            }
            tri[r][k] = val;
        }
    }
    max_row = target;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    size_t r = 0;
    size_t tri_sum = 0;          /* r(r+1)/2 */
    /* while adding next row would exceed n */
    while (tri_sum + r + 1 <= (size_t)n) {
        tri_sum += r + 1;    /* now tri_sum = (r+1)(r+2)/2 */
        ++r;
    }
    size_t k = (size_t)n - tri_sum;   /* position within the row */

    build_triangle_up_to(r);
    return (seq_t)tri[r][k];
}
